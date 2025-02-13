// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL

#include <munit.h>

#include <chiaki/bitstream.h>
#include <stdio.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

static MunitResult test_bitstream_parse_h264(const MunitParameter params[], void *fixture)
{
	ChiakiBitstream bs;
	ChiakiBitstreamSlice slice;

	uint8_t header[] = {
		0x00, 0x00, 0x00, 0x01, 0x67, 0x4d, 0x40, 0x32, 0x91, 0x8a, 0x01, 0xe0, 0x08, 0x9f, 0x97, 0x01,
		0x6a, 0x02, 0x02, 0x02, 0x80, 0x00, 0x03, 0xe9, 0x00, 0x01, 0xd4, 0xc0, 0x44, 0xd0, 0xf1, 0xf1,
		0x50, 0x00, 0x00, 0x00, 0x01, 0x68, 0xee, 0x3c, 0x80,
	};
	memset(&bs, -1, sizeof(bs));
	munit_assert(chiaki_bitstream_header(&bs, header, ARRAY_SIZE(header), CHIAKI_CODEC_H264));
	munit_assert(bs.h264.sps.log2_max_frame_num_minus4 == 3);

	uint8_t slice_i[] = {
		0x00, 0x00, 0x00, 0x01, 0x65, 0x88, 0x80, 0x82, 0x1f, 0x00, 0x49, 0xee, 0x03, 0x29, 0xff, 0xf8,
		0x7f, 0x88, 0x46, 0x44, 0x77, 0x17, 0xe7, 0x6d, 0xb3, 0xad, 0x38, 0x19, 0x74, 0x5a, 0xf1, 0x51,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_i, ARRAY_SIZE(slice_i), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_I);

	uint8_t slice_p[] = {
		0x00, 0x00, 0x00, 0x01, 0x41, 0x9a, 0x04, 0x44, 0x3f, 0x41, 0x5b, 0xf4, 0x65, 0xb4, 0x3e, 0x1a,
		0xd3, 0xa0, 0x28, 0x1f, 0x83, 0x63, 0x0e, 0xc2, 0xfc, 0x9d, 0x7a, 0xc7, 0xc4, 0x7d, 0xf9, 0x18,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p, ARRAY_SIZE(slice_p), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 0);

	uint8_t slice_p_ref_5[] = {
		0x00, 0x00, 0x00, 0x01, 0x41, 0x9b, 0xfd, 0x98, 0x89, 0xdf, 0x00, 0x03, 0x24, 0x60, 0x47, 0x1a,
		0x90, 0x10, 0xb3, 0x2c, 0x4e, 0x45, 0xfc, 0xff, 0x45, 0x24, 0x8c, 0x79, 0xec, 0x12, 0xe5, 0x9b,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p_ref_5, ARRAY_SIZE(slice_p_ref_5), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 5);

	return MUNIT_OK;
}

static MunitResult test_bitstream_parse_h265(const MunitParameter params[], void *fixture)
{
	ChiakiBitstream bs;
	ChiakiBitstreamSlice slice;

	uint8_t header[] = {
		0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
		0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0x0a, 0xc0, 0x90, 0x00, 0x00, 0x00, 0x01,
		0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x00, 0x96, 0xa0, 0x03, 0xc0, 0x80, 0x11, 0x07, 0xcb, 0xc2, 0xb9, 0x24, 0x29, 0x52, 0x70, 0x16,
		0xa0, 0x20, 0x20, 0x20, 0x80, 0x00, 0x07, 0xd2, 0x00, 0x01, 0xd4, 0xc0, 0x20, 0xe5, 0xa1, 0xe3,
		0xd0, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf3, 0xc0, 0x4c, 0x90,
	};
	memset(&bs, -1, sizeof(bs));
	munit_assert(chiaki_bitstream_header(&bs, header, ARRAY_SIZE(header), CHIAKI_CODEC_H265));
	munit_assert(bs.h265.sps.log2_max_pic_order_cnt_lsb_minus4 == 0);

	uint8_t slice_i[] = {
		0x00, 0x00, 0x00, 0x01, 0x28, 0x01, 0xac, 0x25, 0xcf, 0x83, 0xff, 0x23, 0x54, 0xab, 0x5c, 0xf5,
		0x7a, 0x06, 0x7c, 0x3f, 0x31, 0x9b, 0xe6, 0x10, 0x57, 0xe8, 0x0e, 0xcf, 0xdd, 0xda, 0xdb, 0x3f,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_i, ARRAY_SIZE(slice_i), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_I);

	uint8_t slice_p[] = {
		0x00, 0x00, 0x00, 0x01, 0x02, 0x01, 0xd0, 0x97, 0x61, 0x28, 0x23, 0x2d, 0x8b, 0x80, 0x6f, 0xfd,
		0x2f, 0x2b, 0x11, 0xd4, 0x55, 0x04, 0x90, 0x18, 0x49, 0xe5, 0xbc, 0xc4, 0x97, 0xbc, 0x3d, 0xeb,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p, ARRAY_SIZE(slice_p), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 0);

	uint8_t slice_p_ref_5[] = {
		0x00, 0x00, 0x00, 0x01, 0x02, 0x01, 0xd7, 0x85, 0x6a, 0xae, 0xa6, 0x11, 0x80, 0x95, 0x80, 0x0a,
		0xec, 0x5e, 0xdf, 0x39, 0x86, 0xe6, 0xd9, 0x07, 0x49, 0x17, 0xe2, 0x62, 0x57, 0x14, 0xd7, 0x08,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p_ref_5, ARRAY_SIZE(slice_p_ref_5), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 5);

	return MUNIT_OK;
}

static MunitResult test_bitstream_issue_213(const MunitParameter params[], void *fixture)
{
	ChiakiBitstream bs;
	ChiakiBitstreamSlice slice;

	uint8_t header[] = {
		0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
		0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0x0a, 0xc0, 0x90, 0x00, 0x00, 0x00, 0x01,
		0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x00, 0x96, 0xa0, 0x03, 0xc0, 0x80, 0x11, 0x07, 0xcb, 0xc2, 0xb9, 0x24, 0x29, 0x52, 0x70, 0x16,
		0xa0, 0x20, 0x20, 0x20, 0x80, 0x00, 0x07, 0xd2, 0x00, 0x01, 0xd4, 0xc0, 0x20, 0xe5, 0xa1, 0xe3,
		0xd0, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf3, 0xc0, 0x4c, 0x90,
	};
	munit_assert(chiaki_bitstream_header(&bs, header, ARRAY_SIZE(header), CHIAKI_CODEC_H265));

	uint8_t slice_p[] = {
		0x00, 0x00, 0x00, 0x01, 0x02, 0x01, 0xd2, 0x0b, 0xea, 0x60, 0x86, 0x82, 0x3d, 0x00, 0x00, 0x03,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p, ARRAY_SIZE(slice_p), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 0);

	return MUNIT_OK;
}

static MunitResult test_bitstream_set_ref_h265(const MunitParameter params[], void *fixture)
{
	ChiakiBitstream bs;
	ChiakiBitstreamSlice slice;

	uint8_t header[] = {
		0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x0c, 0x01, 0xff, 0xff, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00,
		0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x96, 0x0a, 0xc0, 0x90, 0x00, 0x00, 0x00, 0x01,
		0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0xb0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x00, 0x96, 0xa0, 0x03, 0xc0, 0x80, 0x11, 0x07, 0xcb, 0xc2, 0xb9, 0x24, 0x29, 0x52, 0x70, 0x16,
		0xa0, 0x20, 0x20, 0x20, 0x80, 0x00, 0x07, 0xd2, 0x00, 0x01, 0xd4, 0xc0, 0x20, 0xe5, 0xa1, 0xe3,
		0xd0, 0x00, 0x00, 0x00, 0x01, 0x44, 0x01, 0xc0, 0xf3, 0xc0, 0x4c, 0x90,
	};
	memset(&bs, -1, sizeof(bs));
	munit_assert(chiaki_bitstream_header(&bs, header, ARRAY_SIZE(header), CHIAKI_CODEC_H265));
	munit_assert(bs.h265.sps.log2_max_pic_order_cnt_lsb_minus4 == 0);

	uint8_t slice_p[] = {
		0x00, 0x00, 0x00, 0x01, 0x02, 0x01, 0xd2, 0x85, 0x7a, 0xaa, 0xa6, 0x08, 0x60, 0x13, 0x55, 0x17,
		0x6b, 0x71, 0x72, 0xf9, 0x6e, 0xd4, 0xf2, 0x66, 0x78, 0x0c, 0x12, 0xe7, 0x79, 0xf0, 0xbc, 0xc9,
	};
	memset(&slice, -1, sizeof(slice));
	munit_assert(chiaki_bitstream_slice(&bs, slice_p, ARRAY_SIZE(slice_p), &slice));
	munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
	munit_assert(slice.reference_frame == 0);

	for(unsigned i=0; i<9; i++)
	{
		munit_assert(chiaki_bitstream_slice_set_reference_frame(&bs, slice_p, ARRAY_SIZE(slice_p), i));
		memset(&slice, -1, sizeof(slice));
		munit_assert(chiaki_bitstream_slice(&bs, slice_p, ARRAY_SIZE(slice_p), &slice));
		munit_assert(slice.slice_type == CHIAKI_BITSTREAM_SLICE_P);
		munit_assert(slice.reference_frame == i);
	}
	// Slice have 9 reference frames
	munit_assert(!chiaki_bitstream_slice_set_reference_frame(&bs, slice_p, ARRAY_SIZE(slice_p), 10));

	return MUNIT_OK;
}

MunitTest tests_bitstream[] = {
	{
		"/bitstream_parse_h264",
		test_bitstream_parse_h264,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bitstream_parse_h265",
		test_bitstream_parse_h265,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bitstream_issue_213",
		test_bitstream_issue_213,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bitstream_set_ref_h265",
		test_bitstream_set_ref_h265,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
