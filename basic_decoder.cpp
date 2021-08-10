#include <basic_decoder.h>

#include <utils.h>
#include <log.h>
#include <shift.h>

namespace avlib {

    CBasicDecoder::CBasicDecoder(bool progressBar) :
        CDecoder(progressBar) {
    }

    CBasicDecoder::~CBasicDecoder() {
    }

    bool CBasicDecoder::Decode(CBitstream *pBstr, CSequence *pSeq) {
        struct sos_marker sos;
        pBstr->read_block(&sos, sizeof(sos));
        if(MARKER_TYPE_SOS != sos.type) {
            throw utils::StringFormatException("wrong stream format [can not find start of stream marker]\n");
        }
        log_prop("Huffman", "%s", sos.huffman == HUFFMAN_T_DYNAMIC ? "dynamic" : "static");
        log_prop("Width", "%d", sos.width);
        log_prop("Height", "%d", sos.height);
        log_prop("Frame rate", "%d:%d", sos.frame_rate.nom, sos.frame_rate.denom);
        log_prop("Numer of frames", "%d", sos.frames_number);
        log_prop("Quant coeff", "%d", sos.quant_coeff);
        log_prop("Interpolation scale", "%d", sos.interpolation_scale);
        pSeq->setFormat(IMAGE_TYPE_YUV420, sos.height, sos.width);
        pSeq->setFrameRate(CFrameRate(sos.frame_rate.nom, sos.frame_rate.denom));
        pSeq->setYUV4MPEG(true);
        pSeq->WriteYUV4MPEG();
        pBstr->fill();
        CImage<float> *imgF = new CImage<float>(pSeq->getFormat());
        CImage<float> *imgLast = new CImage<float>(pSeq->getFormat());
        CImage<int16_t> *img = new CImage<int16_t>(pSeq->getFormat());
        CDynamicHuffman<int16_t> *htree = new CDynamicHuffman<int16_t>();
        CIDCT *idct = new CIDCT();
        CIQuant *iquant = new CIQuant();
        iquant->setTables(sos.quant_coeff);
        CIZigZag<int16_t, float> *izigzag = new CIZigZag<int16_t, float>();
        CIRLC<int16_t> *irlc;
        CSize size(pSeq->getFormat().Size, pSeq->getFormat().Align);
        CPredictionInfoTable *predTab = new CPredictionInfoTable(CSize(size.Height / 16, size.Width / 16));
        switch(sos.huffman) {
        case HUFFMAN_T_DYNAMIC:
            irlc = new CDynamicIRLC<int16_t>();
            break;
        case HUFFMAN_T_STATIC:
        default:
            irlc = new CStaticIRLC<int16_t>();
            break;
        }
        CShift<float> *shift = new CShift<float>(128.0f);
        CPrediction *pred = new CPrediction();
        pred->Init(pSeq->getFormat(), sos.interpolation_scale);
        pred->setIFrameITransform(shift);
        sof_marker_t sof;
        for(uint32_t n = 0 ; n < sos.frames_number; n++) {
            printProgressBar(n, sos.frames_number);
            pBstr->fill();
            pBstr->read_block(&sof, sizeof(sof));
            if(sof.type != MARKER_TYPE_SOF || sof.size != sizeof(sof_marker_t)) {
                throw utils::StringFormatException("can not sync frame");
            }
            pred->Decode(predTab, pBstr, (FRAME_TYPE)sof.frame_type);
            irlc->Decode(pBstr, img);
            irlc->Fill(pBstr);
            izigzag->Transform(img, imgF);
            iquant->Transform(imgF, imgF);
            idct->Transform(imgF, imgF);
            pred->ITransform(imgF, imgF, predTab, (FRAME_TYPE)sof.frame_type);
            (*pSeq->getFrame()) = *imgF;
            if(!pSeq->WriteNext()) {
                throw utils::StringFormatException("can not write frame to file");
            }
            (*imgLast) = (*imgF);
        }
        dbg("\n");
        delete shift;
        delete pred;
        delete predTab;
        delete img;
        delete imgF;
        delete imgLast;
        delete idct;
        delete htree;
        delete izigzag;
        delete iquant;
        delete irlc;
        return false;
    }

}
