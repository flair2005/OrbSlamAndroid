/* sgbrfsx.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static integer c_n1 = -1;
static integer c__0 = 0;
static integer c__1 = 1;

/* Subroutine */ int sgbrfsx_(char *trans, char *equed, integer *n, integer *
	kl, integer *ku, integer *nrhs, real *ab, integer *ldab, real *afb, 
	integer *ldafb, integer *ipiv, real *r__, real *c__, real *b, integer 
	*ldb, real *x, integer *ldx, real *rcond, real *berr, integer *
	n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *
	nparams, real *params, real *work, integer *iwork, integer *info)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, afb_dim1, afb_offset, b_dim1, b_offset, 
	    x_dim1, x_offset, err_bnds_norm_dim1, err_bnds_norm_offset, 
	    err_bnds_comp_dim1, err_bnds_comp_offset, i__1;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real illrcond_thresh__, unstable_thresh__, err_lbnd__;
    integer ref_type__;
    extern integer ilatrans_(char *);
    integer j;
    real rcond_tmp__;
    integer prec_type__, trans_type__;
    extern doublereal sla_gbrcond__(char *, integer *, integer *, integer *, 
	    real *, integer *, real *, integer *, integer *, integer *, real *
	    , integer *, real *, integer *, ftnlen);
    real cwise_wrong__;
    extern /* Subroutine */ int sla_gbrfsx_extended__(integer *, integer *, 
	    integer *, integer *, integer *, integer *, real *, integer *, 
	    real *, integer *, integer *, logical *, real *, real *, integer *
	    , real *, integer *, real *, integer *, real *, real *, real *, 
	    real *, real *, real *, real *, integer *, real *, real *, 
	    logical *, integer *);
    char norm[1];
    logical ignore_cwise__;
    extern logical lsame_(char *, char *);
    real anorm;
    extern doublereal slangb_(char *, integer *, integer *, integer *, real *, 
	     integer *, real *), slamch_(char *);
    extern /* Subroutine */ int sgbcon_(char *, integer *, integer *, integer 
	    *, real *, integer *, integer *, real *, real *, real *, integer *
, integer *), xerbla_(char *, integer *);
    logical colequ, notran, rowequ;
    extern integer ilaprec_(char *);
    integer ithresh, n_norms__;
    real rthresh;


/*     -- LAPACK routine (version 3.2.1)                                 -- */
/*     -- Contributed by James Demmel, Deaglan Halligan, Yozo Hida and -- */
/*     -- Jason Riedy of Univ. of California Berkeley.                 -- */
/*     -- April 2009                                                   -- */

/*     -- LAPACK is a software package provided by Univ. of Tennessee, -- */
/*     -- Univ. of California Berkeley and NAG Ltd.                    -- */

/*     .. */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*     Purpose */
/*     ======= */

/*     SGBRFSX improves the computed solution to a system of linear */
/*     equations and provides error bounds and backward error estimates */
/*     for the solution.  In addition to normwise error bound, the code */
/*     provides maximum componentwise error bound if possible.  See */
/*     comments for ERR_BNDS_NORM and ERR_BNDS_COMP for details of the */
/*     error bounds. */

/*     The original system of linear equations may have been equilibrated */
/*     before calling this routine, as described by arguments EQUED, R */
/*     and C below. In this case, the solution and error bounds returned */
/*     are for the original unequilibrated system. */

/*     Arguments */
/*     ========= */

/*     Some optional parameters are bundled in the PARAMS array.  These */
/*     settings determine how refinement is performed, but often the */
/*     defaults are acceptable.  If the defaults are acceptable, users */
/*     can pass NPARAMS = 0 which prevents the source code from accessing */
/*     the PARAMS argument. */

/*     TRANS   (input) CHARACTER*1 */
/*     Specifies the form of the system of equations: */
/*       = 'N':  A * X = B     (No transpose) */
/*       = 'T':  A**T * X = B  (Transpose) */
/*       = 'C':  A**H * X = B  (Conjugate transpose = Transpose) */

/*     EQUED   (input) CHARACTER*1 */
/*     Specifies the form of equilibration that was done to A */
/*     before calling this routine. This is needed to compute */
/*     the solution and error bounds correctly. */
/*       = 'N':  No equilibration */
/*       = 'R':  Row equilibration, i.e., A has been premultiplied by */
/*               diag(R). */
/*       = 'C':  Column equilibration, i.e., A has been postmultiplied */
/*               by diag(C). */
/*       = 'B':  Both row and column equilibration, i.e., A has been */
/*               replaced by diag(R) * A * diag(C). */
/*               The right hand side B has been changed accordingly. */

/*     N       (input) INTEGER */
/*     The order of the matrix A.  N >= 0. */

/*     KL      (input) INTEGER */
/*     The number of subdiagonals within the band of A.  KL >= 0. */

/*     KU      (input) INTEGER */
/*     The number of superdiagonals within the band of A.  KU >= 0. */

/*     NRHS    (input) INTEGER */
/*     The number of right hand sides, i.e., the number of columns */
/*     of the matrices B and X.  NRHS >= 0. */

/*     AB      (input) DOUBLE PRECISION array, dimension (LDAB,N) */
/*     The original band matrix A, stored in rows 1 to KL+KU+1. */
/*     The j-th column of A is stored in the j-th column of the */
/*     array AB as follows: */
/*     AB(ku+1+i-j,j) = A(i,j) for max(1,j-ku)<=i<=min(n,j+kl). */

/*     LDAB    (input) INTEGER */
/*     The leading dimension of the array AB.  LDAB >= KL+KU+1. */

/*     AFB     (input) DOUBLE PRECISION array, dimension (LDAFB,N) */
/*     Details of the LU factorization of the band matrix A, as */
/*     computed by DGBTRF.  U is stored as an upper triangular band */
/*     matrix with KL+KU superdiagonals in rows 1 to KL+KU+1, and */
/*     the multipliers used during the factorization are stored in */
/*     rows KL+KU+2 to 2*KL+KU+1. */

/*     LDAFB   (input) INTEGER */
/*     The leading dimension of the array AFB.  LDAFB >= 2*KL*KU+1. */

/*     IPIV    (input) INTEGER array, dimension (N) */
/*     The pivot indices from SGETRF; for 1<=i<=N, row i of the */
/*     matrix was interchanged with row IPIV(i). */

/*     R       (input or output) REAL array, dimension (N) */
/*     The row scale factors for A.  If EQUED = 'R' or 'B', A is */
/*     multiplied on the left by diag(R); if EQUED = 'N' or 'C', R */
/*     is not accessed.  R is an input argument if FACT = 'F'; */
/*     otherwise, R is an output argument.  If FACT = 'F' and */
/*     EQUED = 'R' or 'B', each element of R must be positive. */
/*     If R is output, each element of R is a power of the radix. */
/*     If R is input, each element of R should be a power of the radix */
/*     to ensure a reliable solution and error estimates. Scaling by */
/*     powers of the radix does not cause rounding errors unless the */
/*     result underflows or overflows. Rounding errors during scaling */
/*     lead to refining with a matrix that is not equivalent to the */
/*     input matrix, producing error estimates that may not be */
/*     reliable. */

/*     C       (input or output) REAL array, dimension (N) */
/*     The column scale factors for A.  If EQUED = 'C' or 'B', A is */
/*     multiplied on the right by diag(C); if EQUED = 'N' or 'R', C */
/*     is not accessed.  C is an input argument if FACT = 'F'; */
/*     otherwise, C is an output argument.  If FACT = 'F' and */
/*     EQUED = 'C' or 'B', each element of C must be positive. */
/*     If C is output, each element of C is a power of the radix. */
/*     If C is input, each element of C should be a power of the radix */
/*     to ensure a reliable solution and error estimates. Scaling by */
/*     powers of the radix does not cause rounding errors unless the */
/*     result underflows or overflows. Rounding errors during scaling */
/*     lead to refining with a matrix that is not equivalent to the */
/*     input matrix, producing error estimates that may not be */
/*     reliable. */

/*     B       (input) REAL array, dimension (LDB,NRHS) */
/*     The right hand side matrix B. */

/*     LDB     (input) INTEGER */
/*     The leading dimension of the array B.  LDB >= max(1,N). */

/*     X       (input/output) REAL array, dimension (LDX,NRHS) */
/*     On entry, the solution matrix X, as computed by SGETRS. */
/*     On exit, the improved solution matrix X. */

/*     LDX     (input) INTEGER */
/*     The leading dimension of the array X.  LDX >= max(1,N). */

/*     RCOND   (output) REAL */
/*     Reciprocal scaled condition number.  This is an estimate of the */
/*     reciprocal Skeel condition number of the matrix A after */
/*     equilibration (if done).  If this is less than the machine */
/*     precision (in particular, if it is zero), the matrix is singular */
/*     to working precision.  Note that the error may still be small even */
/*     if this number is very small and the matrix appears ill- */
/*     conditioned. */

/*     BERR    (output) REAL array, dimension (NRHS) */
/*     Componentwise relative backward error.  This is the */
/*     componentwise relative backward error of each solution vector X(j) */
/*     (i.e., the smallest relative change in any element of A or B that */
/*     makes X(j) an exact solution). */

/*     N_ERR_BNDS (input) INTEGER */
/*     Number of error bounds to return for each right hand side */
/*     and each type (normwise or componentwise).  See ERR_BNDS_NORM and */
/*     ERR_BNDS_COMP below. */

/*     ERR_BNDS_NORM  (output) REAL array, dimension (NRHS, N_ERR_BNDS) */
/*     For each right-hand side, this array contains information about */
/*     various error bounds and condition numbers corresponding to the */
/*     normwise relative error, which is defined as follows: */

/*     Normwise relative error in the ith solution vector: */
/*             max_j (abs(XTRUE(j,i) - X(j,i))) */
/*            ------------------------------ */
/*                  max_j abs(X(j,i)) */

/*     The array is indexed by the type of error information as described */
/*     below. There currently are up to three pieces of information */
/*     returned. */

/*     The first index in ERR_BNDS_NORM(i,:) corresponds to the ith */
/*     right-hand side. */

/*     The second index in ERR_BNDS_NORM(:,err) contains the following */
/*     three fields: */
/*     err = 1 "Trust/don't trust" boolean. Trust the answer if the */
/*              reciprocal condition number is less than the threshold */
/*              sqrt(n) * slamch('Epsilon'). */

/*     err = 2 "Guaranteed" error bound: The estimated forward error, */
/*              almost certainly within a factor of 10 of the true error */
/*              so long as the next entry is greater than the threshold */
/*              sqrt(n) * slamch('Epsilon'). This error bound should only */
/*              be trusted if the previous boolean is true. */

/*     err = 3  Reciprocal condition number: Estimated normwise */
/*              reciprocal condition number.  Compared with the threshold */
/*              sqrt(n) * slamch('Epsilon') to determine if the error */
/*              estimate is "guaranteed". These reciprocal condition */
/*              numbers are 1 / (norm(Z^{-1},inf) * norm(Z,inf)) for some */
/*              appropriately scaled matrix Z. */
/*              Let Z = S*A, where S scales each row by a power of the */
/*              radix so all absolute row sums of Z are approximately 1. */

/*     See Lapack Working Note 165 for further details and extra */
/*     cautions. */

/*     ERR_BNDS_COMP  (output) REAL array, dimension (NRHS, N_ERR_BNDS) */
/*     For each right-hand side, this array contains information about */
/*     various error bounds and condition numbers corresponding to the */
/*     componentwise relative error, which is defined as follows: */

/*     Componentwise relative error in the ith solution vector: */
/*                    abs(XTRUE(j,i) - X(j,i)) */
/*             max_j ---------------------- */
/*                         abs(X(j,i)) */

/*     The array is indexed by the right-hand side i (on which the */
/*     componentwise relative error depends), and the type of error */
/*     information as described below. There currently are up to three */
/*     pieces of information returned for each right-hand side. If */
/*     componentwise accuracy is not requested (PARAMS(3) = 0.0), then */
/*     ERR_BNDS_COMP is not accessed.  If N_ERR_BNDS .LT. 3, then at most */
/*     the first (:,N_ERR_BNDS) entries are returned. */

/*     The first index in ERR_BNDS_COMP(i,:) corresponds to the ith */
/*     right-hand side. */

/*     The second index in ERR_BNDS_COMP(:,err) contains the following */
/*     three fields: */
/*     err = 1 "Trust/don't trust" boolean. Trust the answer if the */
/*              reciprocal condition number is less than the threshold */
/*              sqrt(n) * slamch('Epsilon'). */

/*     err = 2 "Guaranteed" error bound: The estimated forward error, */
/*              almost certainly within a factor of 10 of the true error */
/*              so long as the next entry is greater than the threshold */
/*              sqrt(n) * slamch('Epsilon'). This error bound should only */
/*              be trusted if the previous boolean is true. */

/*     err = 3  Reciprocal condition number: Estimated componentwise */
/*              reciprocal condition number.  Compared with the threshold */
/*              sqrt(n) * slamch('Epsilon') to determine if the error */
/*              estimate is "guaranteed". These reciprocal condition */
/*              numbers are 1 / (norm(Z^{-1},inf) * norm(Z,inf)) for some */
/*              appropriately scaled matrix Z. */
/*              Let Z = S*(A*diag(x)), where x is the solution for the */
/*              current right-hand side and S scales each row of */
/*              A*diag(x) by a power of the radix so all absolute row */
/*              sums of Z are approximately 1. */

/*     See Lapack Working Note 165 for further details and extra */
/*     cautions. */

/*     NPARAMS (input) INTEGER */
/*     Specifies the number of parameters set in PARAMS.  If .LE. 0, the */
/*     PARAMS array is never referenced and default values are used. */

/*     PARAMS  (input / output) REAL array, dimension NPARAMS */
/*     Specifies algorithm parameters.  If an entry is .LT. 0.0, then */
/*     that entry will be filled with default value used for that */
/*     parameter.  Only positions up to NPARAMS are accessed; defaults */
/*     are used for higher-numbered parameters. */

/*       PARAMS(LA_LINRX_ITREF_I = 1) : Whether to perform iterative */
/*            refinement or not. */
/*         Default: 1.0 */
/*            = 0.0 : No refinement is performed, and no error bounds are */
/*                    computed. */
/*            = 1.0 : Use the double-precision refinement algorithm, */
/*                    possibly with doubled-single computations if the */
/*                    compilation environment does not support DOUBLE */
/*                    PRECISION. */
/*              (other values are reserved for future use) */

/*       PARAMS(LA_LINRX_ITHRESH_I = 2) : Maximum number of residual */
/*            computations allowed for refinement. */
/*         Default: 10 */
/*         Aggressive: Set to 100 to permit convergence using approximate */
/*                     factorizations or factorizations other than LU. If */
/*                     the factorization uses a technique other than */
/*                     Gaussian elimination, the guarantees in */
/*                     err_bnds_norm and err_bnds_comp may no longer be */
/*                     trustworthy. */

/*       PARAMS(LA_LINRX_CWISE_I = 3) : Flag determining if the code */
/*            will attempt to find a solution with small componentwise */
/*            relative error in the double-precision algorithm.  Positive */
/*            is true, 0.0 is false. */
/*         Default: 1.0 (attempt componentwise convergence) */

/*     WORK    (workspace) REAL array, dimension (4*N) */

/*     IWORK   (workspace) INTEGER array, dimension (N) */

/*     INFO    (output) INTEGER */
/*       = 0:  Successful exit. The solution to every right-hand side is */
/*         guaranteed. */
/*       < 0:  If INFO = -i, the i-th argument had an illegal value */
/*       > 0 and <= N:  U(INFO,INFO) is exactly zero.  The factorization */
/*         has been completed, but the factor U is exactly singular, so */
/*         the solution and error bounds could not be computed. RCOND = 0 */
/*         is returned. */
/*       = N+J: The solution corresponding to the Jth right-hand side is */
/*         not guaranteed. The solutions corresponding to other right- */
/*         hand sides K with K > J may not be guaranteed as well, but */
/*         only the first such right-hand side is reported. If a small */
/*         componentwise error is not requested (PARAMS(3) = 0.0) then */
/*         the Jth right-hand side is the first with a normwise error */
/*         bound that is not guaranteed (the smallest J such */
/*         that ERR_BNDS_NORM(J,1) = 0.0). By default (PARAMS(3) = 1.0) */
/*         the Jth right-hand side is the first with either a normwise or */
/*         componentwise error bound that is not guaranteed (the smallest */
/*         J such that either ERR_BNDS_NORM(J,1) = 0.0 or */
/*         ERR_BNDS_COMP(J,1) = 0.0). See the definition of */
/*         ERR_BNDS_NORM(:,1) and ERR_BNDS_COMP(:,1). To get information */
/*         about all of the right-hand sides check ERR_BNDS_NORM or */
/*         ERR_BNDS_COMP. */

/*     ================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Check the input parameters. */

    /* Parameter adjustments */
    err_bnds_comp_dim1 = *nrhs;
    err_bnds_comp_offset = 1 + err_bnds_comp_dim1;
    err_bnds_comp__ -= err_bnds_comp_offset;
    err_bnds_norm_dim1 = *nrhs;
    err_bnds_norm_offset = 1 + err_bnds_norm_dim1;
    err_bnds_norm__ -= err_bnds_norm_offset;
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    afb_dim1 = *ldafb;
    afb_offset = 1 + afb_dim1;
    afb -= afb_offset;
    --ipiv;
    --r__;
    --c__;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --berr;
    --params;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    trans_type__ = ilatrans_(trans);
    ref_type__ = 1;
    if (*nparams >= 1) {
	if (params[1] < 0.f) {
	    params[1] = 1.f;
	} else {
	    ref_type__ = params[1];
	}
    }

/*     Set default parameters. */

    illrcond_thresh__ = (real) (*n) * slamch_("Epsilon");
    ithresh = 10;
    rthresh = .5f;
    unstable_thresh__ = .25f;
    ignore_cwise__ = FALSE_;

    if (*nparams >= 2) {
	if (params[2] < 0.f) {
	    params[2] = (real) ithresh;
	} else {
	    ithresh = (integer) params[2];
	}
    }
    if (*nparams >= 3) {
	if (params[3] < 0.f) {
	    if (ignore_cwise__) {
		params[3] = 0.f;
	    } else {
		params[3] = 1.f;
	    }
	} else {
	    ignore_cwise__ = params[3] == 0.f;
	}
    }
    if (ref_type__ == 0 || *n_err_bnds__ == 0) {
	n_norms__ = 0;
    } else if (ignore_cwise__) {
	n_norms__ = 1;
    } else {
	n_norms__ = 2;
    }

    notran = lsame_(trans, "N");
    rowequ = lsame_(equed, "R") || lsame_(equed, "B");
    colequ = lsame_(equed, "C") || lsame_(equed, "B");

/*     Test input parameters. */

    if (trans_type__ == -1) {
	*info = -1;
    } else if (! rowequ && ! colequ && ! lsame_(equed, "N")) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*kl < 0) {
	*info = -4;
    } else if (*ku < 0) {
	*info = -5;
    } else if (*nrhs < 0) {
	*info = -6;
    } else if (*ldab < *kl + *ku + 1) {
	*info = -8;
    } else if (*ldafb < (*kl << 1) + *ku + 1) {
	*info = -10;
    } else if (*ldb < max(1,*n)) {
	*info = -13;
    } else if (*ldx < max(1,*n)) {
	*info = -15;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGBRFSX", &i__1);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0 || *nrhs == 0) {
	*rcond = 1.f;
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    berr[j] = 0.f;
	    if (*n_err_bnds__ >= 1) {
		err_bnds_norm__[j + err_bnds_norm_dim1] = 1.f;
		err_bnds_comp__[j + err_bnds_comp_dim1] = 1.f;
	    } else if (*n_err_bnds__ >= 2) {
		err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] = 0.f;
		err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] = 0.f;
	    } else if (*n_err_bnds__ >= 3) {
		err_bnds_norm__[j + err_bnds_norm_dim1 * 3] = 1.f;
		err_bnds_comp__[j + err_bnds_comp_dim1 * 3] = 1.f;
	    }
	}
	return 0;
    }

/*     Default to failure. */

    *rcond = 0.f;
    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	berr[j] = 1.f;
	if (*n_err_bnds__ >= 1) {
	    err_bnds_norm__[j + err_bnds_norm_dim1] = 1.f;
	    err_bnds_comp__[j + err_bnds_comp_dim1] = 1.f;
	} else if (*n_err_bnds__ >= 2) {
	    err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] = 1.f;
	    err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] = 1.f;
	} else if (*n_err_bnds__ >= 3) {
	    err_bnds_norm__[j + err_bnds_norm_dim1 * 3] = 0.f;
	    err_bnds_comp__[j + err_bnds_comp_dim1 * 3] = 0.f;
	}
    }

/*     Compute the norm of A and the reciprocal of the condition */
/*     number of A. */

    if (notran) {
	*(unsigned char *)norm = 'I';
    } else {
	*(unsigned char *)norm = '1';
    }
    anorm = slangb_(norm, n, kl, ku, &ab[ab_offset], ldab, &work[1]);
    sgbcon_(norm, n, kl, ku, &afb[afb_offset], ldafb, &ipiv[1], &anorm, rcond, 
	     &work[1], &iwork[1], info);

/*     Perform refinement on each right-hand side */

    if (ref_type__ != 0) {
	prec_type__ = ilaprec_("D");
	if (notran) {
	    sla_gbrfsx_extended__(&prec_type__, &trans_type__, n, kl, ku, 
		    nrhs, &ab[ab_offset], ldab, &afb[afb_offset], ldafb, &
		    ipiv[1], &colequ, &c__[1], &b[b_offset], ldb, &x[x_offset]
		    , ldx, &berr[1], &n_norms__, &err_bnds_norm__[
		    err_bnds_norm_offset], &err_bnds_comp__[
		    err_bnds_comp_offset], &work[*n + 1], &work[1], &work[(*n 
		    << 1) + 1], &work[1], rcond, &ithresh, &rthresh, &
		    unstable_thresh__, &ignore_cwise__, info);
	} else {
	    sla_gbrfsx_extended__(&prec_type__, &trans_type__, n, kl, ku, 
		    nrhs, &ab[ab_offset], ldab, &afb[afb_offset], ldafb, &
		    ipiv[1], &rowequ, &r__[1], &b[b_offset], ldb, &x[x_offset]
		    , ldx, &berr[1], &n_norms__, &err_bnds_norm__[
		    err_bnds_norm_offset], &err_bnds_comp__[
		    err_bnds_comp_offset], &work[*n + 1], &work[1], &work[(*n 
		    << 1) + 1], &work[1], rcond, &ithresh, &rthresh, &
		    unstable_thresh__, &ignore_cwise__, info);
	}
    }
/* Computing MAX */
    r__1 = 10.f, r__2 = sqrt((real) (*n));
    err_lbnd__ = dmax(r__1,r__2) * slamch_("Epsilon");
    if (*n_err_bnds__ >= 1 && n_norms__ >= 1) {

/*     Compute scaled normwise condition number cond(A*C). */

	if (colequ && notran) {
	    rcond_tmp__ = sla_gbrcond__(trans, n, kl, ku, &ab[ab_offset], 
		    ldab, &afb[afb_offset], ldafb, &ipiv[1], &c_n1, &c__[1], 
		    info, &work[1], &iwork[1], (ftnlen)1);
	} else if (rowequ && ! notran) {
	    rcond_tmp__ = sla_gbrcond__(trans, n, kl, ku, &ab[ab_offset], 
		    ldab, &afb[afb_offset], ldafb, &ipiv[1], &c_n1, &r__[1], 
		    info, &work[1], &iwork[1], (ftnlen)1);
	} else {
	    rcond_tmp__ = sla_gbrcond__(trans, n, kl, ku, &ab[ab_offset], 
		    ldab, &afb[afb_offset], ldafb, &ipiv[1], &c__0, &r__[1], 
		    info, &work[1], &iwork[1], (ftnlen)1);
	}
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {

/*     Cap the error at 1.0. */

	    if (*n_err_bnds__ >= 2 && err_bnds_norm__[j + (err_bnds_norm_dim1 
		    << 1)] > 1.f) {
		err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] = 1.f;
	    }

/*     Threshold the error (see LAWN). */

	    if (rcond_tmp__ < illrcond_thresh__) {
		err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] = 1.f;
		err_bnds_norm__[j + err_bnds_norm_dim1] = 0.f;
		if (*info <= *n) {
		    *info = *n + j;
		}
	    } else if (err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] < 
		    err_lbnd__) {
		err_bnds_norm__[j + (err_bnds_norm_dim1 << 1)] = err_lbnd__;
		err_bnds_norm__[j + err_bnds_norm_dim1] = 1.f;
	    }

/*     Save the condition number. */

	    if (*n_err_bnds__ >= 3) {
		err_bnds_norm__[j + err_bnds_norm_dim1 * 3] = rcond_tmp__;
	    }
	}
    }
    if (*n_err_bnds__ >= 1 && n_norms__ >= 2) {

/*     Compute componentwise condition number cond(A*diag(Y(:,J))) for */
/*     each right-hand side using the current solution as an estimate of */
/*     the true solution.  If the componentwise error estimate is too */
/*     large, then the solution is a lousy estimate of truth and the */
/*     estimated RCOND may be too optimistic.  To avoid misleading users, */
/*     the inverse condition number is set to 0.0 when the estimated */
/*     cwise error is at least CWISE_WRONG. */

	cwise_wrong__ = sqrt(slamch_("Epsilon"));
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    if (err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] < 
		    cwise_wrong__) {
		rcond_tmp__ = sla_gbrcond__(trans, n, kl, ku, &ab[ab_offset], 
			ldab, &afb[afb_offset], ldafb, &ipiv[1], &c__1, &x[j *
			 x_dim1 + 1], info, &work[1], &iwork[1], (ftnlen)1);
	    } else {
		rcond_tmp__ = 0.f;
	    }

/*     Cap the error at 1.0. */

	    if (*n_err_bnds__ >= 2 && err_bnds_comp__[j + (err_bnds_comp_dim1 
		    << 1)] > 1.f) {
		err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] = 1.f;
	    }

/*     Threshold the error (see LAWN). */

	    if (rcond_tmp__ < illrcond_thresh__) {
		err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] = 1.f;
		err_bnds_comp__[j + err_bnds_comp_dim1] = 0.f;
		if (params[3] == 1.f && *info < *n + j) {
		    *info = *n + j;
		}
	    } else if (err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] < 
		    err_lbnd__) {
		err_bnds_comp__[j + (err_bnds_comp_dim1 << 1)] = err_lbnd__;
		err_bnds_comp__[j + err_bnds_comp_dim1] = 1.f;
	    }

/*     Save the condition number. */

	    if (*n_err_bnds__ >= 3) {
		err_bnds_comp__[j + err_bnds_comp_dim1 * 3] = rcond_tmp__;
	    }
	}
    }

    return 0;

/*     End of SGBRFSX */

} /* sgbrfsx_ */
