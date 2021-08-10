#ifndef _RLC_H
#define _RLC_H

#include <dynamic_huffman.h>
#include <image.h>
#include <mtimer.h>
#include <prediction.h>

namespace avlib {

    template <class T>
    class CRLC : public utils::ITimer {
    public:
        CRLC();
        virtual ~CRLC();
        virtual void Encode(CImage<T> *pImg, CBitstream *pBstr);
        virtual void EncodeBlock(const T *pSrc, CPoint p, CSize s, CBitstream *pBstr) = 0;
        virtual void Flush(CBitstream *bstr) {  }
    protected:
        virtual void doEncode(CImage<T> *pImg, CBitstream *pBstr);
        CDynamicHuffman<int32_t> *m_predHuff;
    };

    template <class T>
    class CIRLC {
    public:
        CIRLC();
        virtual ~CIRLC();
        virtual void Decode(CBitstream *pBstr, CImage<T> *pImg);
        virtual void DecodeBlock(CBitstream *pBstr, T *pDst, CPoint p, CSize s) = 0;
        virtual void Fill(CBitstream *bstr) { }
    protected:
        CDynamicHuffman<int32_t> *m_predHuff;
    };

}

#endif //_RLC_H
