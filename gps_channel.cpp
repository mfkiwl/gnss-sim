// This file is part of:
// gnss-sim: A GNSS Signal Simulator
//
// Copyright (c) 2020 Damien Dusha
// SPDX-License-Identifier: MIT
//
// Derived from https://github.com/osqzss/gps-sdr-sim (MIT Licence):
// Copyright (c) 2015-2020 Takuji Ebinuma

#include "gps_channel.h"

#include <cassert>

namespace {

/*! \brief generate the C/A code sequence for a given Satellite Vehicle PRN
 *  \param[in] prn PRN nuber of the Satellite Vehicle
 *  \param[out] ca Caller-allocated integer array of 1023 bytes
 */
std::array<int, CA_SEQ_LEN> GeneratePrnSequence(int prn)
{
    assert(prn >= 1 && prn <= 32);
    std::array<int, CA_SEQ_LEN> ca;

	constexpr std::array<int, 32> delay = {
		  5,   6,   7,   8,  17,  18, 139, 140, 141, 251,
		252, 254, 255, 256, 257, 258, 469, 470, 471, 472,
		473, 474, 509, 512, 513, 514, 515, 516, 859, 860,
		861, 862};
	
	int g1[CA_SEQ_LEN], g2[CA_SEQ_LEN];
	int r1[N_DWRD_SBF], r2[N_DWRD_SBF];

	for (int i = 0; i < N_DWRD_SBF; i++) {
		r1[i] = r2[i] = -1;
    }

	for (int i = 0; i < CA_SEQ_LEN; i++)
	{
		g1[i] = r1[9];
		g2[i] = r2[9];
		int c1 = r1[2]*r1[9];
		int c2 = r2[1]*r2[2]*r2[5]*r2[7]*r2[8]*r2[9];

		for (int j = 9; j > 0; j--) 
		{
			r1[j] = r1[j-1];
			r2[j] = r2[j-1];
		}
		r1[0] = c1;
		r2[0] = c2;
	}

	for (int i = 0, j = CA_SEQ_LEN-delay[prn-1]; i < CA_SEQ_LEN; i++,j++) {
		ca[i] = (1-g1[i]*g2[j%CA_SEQ_LEN])/2 == 0 ? -1 : 1;
    }

	return ca;
}

}   // namespace

GpsChannel::GpsChannel(int in_prn)
    : prn(in_prn)
    , code_sequence_(GeneratePrnSequence(in_prn))
{
}
