#ifndef _ENCODER_SAD_H_
#define _ENCODER_SAD_H_


#include "../portab.h"

typedef void (sadInitFunc) (void);
typedef sadInitFunc *sadInitFuncPtr;

extern sadInitFuncPtr sadInit;
sadInitFunc sadInit_altivec;


typedef uint32_t(sad16Func) (const uint8_t * const cur,
							 const uint8_t * const ref,
							 const uint32_t stride,
							 const uint32_t best_sad);
typedef sad16Func *sad16FuncPtr;
extern sad16FuncPtr sad16;
sad16Func sad16_c;
sad16Func sad16_mmx;
sad16Func sad16_xmm;
sad16Func sad16_3dne;
sad16Func sad16_sse2;
sad16Func sad16_altivec;
sad16Func sad16_ia64;

sad16Func mrsad16_c;

typedef uint32_t(sad8Func) (const uint8_t * const cur,
							const uint8_t * const ref,
							const uint32_t stride);
typedef sad8Func *sad8FuncPtr;
extern sad8FuncPtr sad8;
sad8Func sad8_c;
sad8Func sad8_mmx;
sad8Func sad8_xmm;
sad8Func sad8_3dne;
sad8Func sad8_altivec;
sad8Func sad8_ia64;


typedef uint32_t(sad16biFunc) (const uint8_t * const cur,
							   const uint8_t * const ref1,
							   const uint8_t * const ref2,
							   const uint32_t stride);
typedef sad16biFunc *sad16biFuncPtr;
extern sad16biFuncPtr sad16bi;
sad16biFunc sad16bi_c;
sad16biFunc sad16bi_ia64;
sad16biFunc sad16bi_mmx;
sad16biFunc sad16bi_xmm;
sad16biFunc sad16bi_3dne;
sad16biFunc sad16bi_3dn;


typedef uint32_t(sad8biFunc) (const uint8_t * const cur,
							   const uint8_t * const ref1,
							   const uint8_t * const ref2,
							   const uint32_t stride);
typedef sad8biFunc *sad8biFuncPtr;
extern sad8biFuncPtr sad8bi;
sad8biFunc sad8bi_c;
sad8biFunc sad8bi_mmx;
sad8biFunc sad8bi_xmm;
sad8biFunc sad8bi_3dne;
sad8biFunc sad8bi_3dn;


typedef uint32_t(dev16Func) (const uint8_t * const cur,
							 const uint32_t stride);
typedef dev16Func *dev16FuncPtr;
extern dev16FuncPtr dev16;
dev16Func dev16_c;
dev16Func dev16_mmx;
dev16Func dev16_xmm;
dev16Func dev16_3dne;
dev16Func dev16_sse2;
dev16Func dev16_altivec;
dev16Func dev16_ia64;

typedef uint32_t (sad16vFunc)(	const uint8_t * const cur,
								const uint8_t * const ref,
								const uint32_t stride, int32_t *sad8);
typedef sad16vFunc *sad16vFuncPtr;
extern sad16vFuncPtr sad16v;
sad16vFunc sad16v_xmm;
sad16vFunc sad16v_mmx;
sad16vFunc sad16v_c;
sad16vFunc sad32v_c;
sad16vFunc mrsad16v;
sad16vFunc mrsad16v_c;

int32_t sad8x8mean_mmx(	const uint8_t * const current,
						const uint8_t * const reference,
						const uint32_t stride,
						const int mean);

void sad16x8total_mmx(const uint8_t *, const uint32_t, int32_t[]);

#endif							/* _ENCODER_SAD_H_ */