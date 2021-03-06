#ifndef HAVE_SSE2_DEFINES
#define HAVE_SSE2_DEFINES
typedef int v4si __attribute__ ((vector_size (16)));
typedef short v8hi __attribute__ ((vector_size (16)));
typedef unsigned char v16qi __attribute__ ((vector_size (16)));
#endif

static int
NAME (const int *arr, int n, int key)
{
	v4si *in_data = (v4si*)arr;
	int i = 0;
	int res;
	v4si key4 = { key, key, key, key };

#ifdef NO_BRANCH
	for (;;) {
	    v4si cmp0 = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 0]);
	    v4si cmp1 = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 1]);
	    v4si cmp2 = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 2]);
	    v4si cmp3 = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 3]);

	    v8hi pack01 = __builtin_ia32_packssdw128 (cmp0, cmp1);
	    v8hi pack23 = __builtin_ia32_packssdw128 (cmp2, cmp3);
	    v16qi pack0123 = __builtin_ia32_packsswb128 (pack01, pack23);

	    res = __builtin_ia32_pmovmskb128 (pack0123);

	    if (res != 0xffff)
		break;

	    i += 4;
	}

	return i * 4 + __builtin_ctz (~res);
#else
	for (;;) {
		v4si tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 0]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff)
			break;

#if defined (UNROLL2)
		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 1]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 1;
			break;
		}

		i += 2;
#elif defined (UNROLL4)
		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 1]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 1;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 2]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 2;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 3]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 3;
			break;
		}

		i += 4;
#elif defined (UNROLL8)
		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 1]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 1;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 2]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 2;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 3]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 3;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 4]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 4;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 5]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 5;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 6]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 6;
			break;
		}

		tmp = __builtin_ia32_pcmpgtd128 (key4, in_data [i + 7]);
		res = __builtin_ia32_pmovmskb128 ((v16qi)tmp);
		if (res != 0xffff) {
			i += 7;
			break;
		}

		i += 8;
#else
		++i;
#endif
	}

	return i * 4 + __builtin_ctz (~res) / 4;
#endif
}

#undef NAME
#undef UNROLL2
#undef UNROLL4
#undef UNROLL8
