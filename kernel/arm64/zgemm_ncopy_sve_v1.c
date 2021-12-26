/*********************************************************************/
/* Copyright 2009, 2010 The University of Texas at Austin.           */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#include <stdio.h>
#include "common.h"
#include <arm_sve.h>

// TODO: write in assembly with proper unrolling of inner loop
int CNAME(BLASLONG m, BLASLONG n, IFLOAT *a, BLASLONG lda, IFLOAT *b){

    BLASLONG j;
    IFLOAT *aoffset, *aoffset1, *boffset;

    svint64_t lda_vec = svindex_s64(0LL, lda * 2);
    uint64_t sve_size = svcntd();

    aoffset = a;
    boffset = b;

    j = 0;
    svbool_t pg = svwhilelt_b64(j, n);
    uint64_t active = svcntp_b64(svptrue_b64(), pg);
    do {

        aoffset1 = aoffset;

        uint64_t i_cnt = m;
        while (i_cnt--) {
            svfloat64_t a_vec_real = svld1_gather_index(pg, (double *) aoffset1, lda_vec);
            svfloat64_t a_vec_imag = svld1_gather_index(pg, ((double *) aoffset1) + 1, lda_vec);
            svst2_f64(pg, (double *) boffset, svcreate2(a_vec_real, a_vec_imag));
            aoffset1 += 2;
            boffset += active;
        }
        aoffset += sve_size * lda * 2;

        j += svcntd();
        pg = svwhilelt_b64(j, n);
        active = svcntp_b64(svptrue_b64(), pg);


    } while (svptest_any(svptrue_b64(), pg));

    return 0;
}
