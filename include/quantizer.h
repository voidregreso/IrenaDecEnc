#ifndef _QUANTIZER_H
#define _QUANTIZER_H

#include <image.h>
#include <stdint.h>
#include <transform.h>

namespace avlib {

    extern const uint8_t default_YQTable[];
    extern const uint8_t default_UQTable[];
    extern const uint8_t default_VQTable[];

    class CQuant : public CTransform<float, float> {
    public:
        CQuant();
        virtual ~CQuant();
        virtual void TransformBlock(float *pSrc, float *pDst, CPoint p, CSize s);
        virtual void setTables(int qp);
        virtual void setTables(const uint8_t *YQ, const uint8_t *UQ, const uint8_t *VQ, int qp);
    protected:
        CImage<float> *m_q;
    };

    class CIQuant : public CTransform<float, float> {
    public:
        CIQuant();
        virtual void TransformBlock(float *pSrc, float *pDst, CPoint p, CSize s);
        virtual void setTables(int qp);
        virtual void setTables(const uint8_t *YQ, const uint8_t *UQ, const uint8_t *VQ, int qp);
    protected:
        CImage<float> *m_q;
    };

}

#endif //_QUANTIZER_H
