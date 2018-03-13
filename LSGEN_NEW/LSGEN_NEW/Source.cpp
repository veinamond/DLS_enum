#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iterator>
#include <omp.h>
static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
using namespace std;
const int N = 10;
const uint32_t MaxN = 1 << N;
const uint32_t AllN = MaxN - 1;

int cur_cnt = 0;

/*int32_t LS[N*N];
int32_t CR[N*N];
int32_t L[N*N];
int32_t CNT[N*N];
uint64_t SquaresCnt = 0;
uint32_t Columns[N], Rows[N], MD, AD;*/



vector<vector<int>> Generate_WU() {
	vector<vector<int32_t>> RES;
	int32_t LS[N*N];
	int32_t CR[N*N];
	uint32_t Columns[N], Rows[N], MD, AD;
	int* trf = new int[1 << N];
	int* trf2 = new int[1 << N];

	for (int i = 0; i < N; i++) { trf[1 << i] = 1 << (N - 1 - i); }
	for (int i = 0; i < N; i++) { trf2[1 << i] = (1 << i) | (1 << (N - 1 - i)); }

	for (int i = 0; i < N*N; i++)
		LS[i] = 0;
	for (int i = 0; i < N; i++)
		LS[i] = 1 << i;
	for (int i = 0; i < N; i++)
		Columns[i] = 0;
	for (int i = 0; i < N; i++)
		Rows[i] = 0;
	MD = 0;
	AD = 0;
	for (int j = 0; j < N; j++) {
		LS[j] = 1 << j;
		Rows[0] |= LS[j];
		Columns[j] |= LS[j];
	}
	MD |= LS[0];
	AD |= LS[N - 1];

	for (CR[11] = AllN ^ (Rows[1] | Columns[1] | MD); CR[11] != 0; CR[11] = CR[11] = (CR[11] & (CR[11] - 1))) {
		LS[11] = (CR[11] & (-CR[11]));
		Rows[1] |= trf2[LS[11]];
		Columns[1] |= LS[11];
		MD |= LS[11];
		AD |= trf[LS[11]];
		for (CR[81] = AllN ^ (Rows[8] | Columns[1] | AD); CR[81] != 0; CR[81] = CR[81] = (CR[81] & (CR[81] - 1))) {
			LS[81] = (CR[81] & (-CR[81]));
			Rows[8] |= trf2[LS[81]];
			Columns[1] |= LS[81];
			AD |= LS[81];
			MD |= trf[LS[81]];
			for (CR[22] = AllN ^ (Rows[2] | Columns[2] | MD); CR[22] != 0; CR[22] = CR[22] = (CR[22] & (CR[22] - 1))) {
				LS[22] = (CR[22] & (-CR[22]));
				Rows[2] |= trf2[LS[22]];
				Columns[2] |= LS[22];
				MD |= LS[22];
				AD |= trf[LS[22]];
				for (CR[72] = AllN ^ (Rows[7] | Columns[2] | AD); CR[72] != 0; CR[72] = CR[72] = (CR[72] & (CR[72] - 1))) {
					LS[72] = (CR[72] & (-CR[72]));
					Rows[7] |= trf2[LS[72]];
					Columns[2] |= LS[72];
					AD |= LS[72];
					MD |= trf[LS[72]];
					for (CR[33] = AllN ^ (Rows[3] | Columns[3] | MD); CR[33] != 0; CR[33] = CR[33] = (CR[33] & (CR[33] - 1))) {
						LS[33] = (CR[33] & (-CR[33]));
						Rows[3] |= trf2[LS[33]];
						Columns[3] |= LS[33];
						MD |= LS[33];
						AD |= trf[LS[33]];
						for (CR[63] = AllN ^ (Rows[6] | Columns[3] | AD); CR[63] != 0; CR[63] = CR[63] = (CR[63] & (CR[63] - 1))) {
							LS[63] = (CR[63] & (-CR[63]));
							Rows[6] |= trf2[LS[63]];
							Columns[3] |= LS[63];
							AD |= LS[63];
							MD |= trf[LS[63]];
							for (CR[44] = AllN ^ (Rows[4] | Columns[4] | MD); CR[44] != 0; CR[44] = CR[44] = (CR[44] & (CR[44] - 1))) {
								LS[44] = (CR[44] & (-CR[44]));
								Rows[4] |= trf2[LS[44]];
								Columns[4] |= LS[44];
								MD |= LS[44];
								AD |= trf[LS[44]];
								for (CR[54] = AllN ^ (Rows[5] | Columns[4] | AD); CR[54] != 0; CR[54] = CR[54] = (CR[54] & (CR[54] - 1))) {
									LS[54] = (CR[54] & (-CR[54]));
									Rows[5] |= trf2[LS[54]];
									Columns[4] |= LS[54];
									AD |= LS[54];
									MD |= trf[LS[54]];
									CR[90] = AllN ^ (Rows[9] | Columns[0] | AD);
									if (CR[90] != 0) {

										LS[90] = (CR[90] & (-CR[90]));
										Rows[9] |= trf2[LS[90]];
										Columns[0] |= LS[90];
										//81088
										for (CR[91] = AllN ^ (Rows[9] | Columns[1]); CR[91] != 0; CR[91] = CR[91] = (CR[91] & (CR[91] - 1))) {
											LS[91] = (CR[91] & (-CR[91]));
											Rows[9] |= trf2[LS[91]];
											Columns[1] |= LS[91];
											for (CR[92] = AllN ^ (Rows[9] | Columns[2]); CR[92] != 0; CR[92] = CR[92] = (CR[92] & (CR[92] - 1))) {
												LS[92] = (CR[92] & (-CR[92]));
												Rows[9] |= trf2[LS[92]];
												Columns[2] |= LS[92];
												for (CR[93] = AllN ^ (Rows[9] | Columns[3]); CR[93] != 0; CR[93] = CR[93] = (CR[93] & (CR[93] - 1))) {
													LS[93] = (CR[93] & (-CR[93]));
													Rows[9] |= trf2[LS[93]];
													Columns[3] |= LS[93];
													for (CR[94] = AllN ^ (Rows[9] | Columns[4]); CR[94] != 0; CR[94] = CR[94] = (CR[94] & (CR[94] - 1))) {
														LS[94] = (CR[94] & (-CR[94]));
														Rows[9] |= trf2[LS[94]];
														Columns[4] |= LS[94];

														vector<int32_t> t = { LS[11],LS[22],LS[33],LS[44],LS[54],LS[63],LS[72], LS[81], LS[90], LS[91], LS[92], LS[93], LS[94] };
														RES.push_back(t);

														Rows[9] ^= trf2[LS[94]];
														Columns[4] ^= LS[94];
													}
													Rows[9] ^= trf2[LS[93]];
													Columns[3] ^= LS[93];
												}
												Rows[9] ^= trf2[LS[92]];
												Columns[2] ^= LS[92];
											}
											Rows[9] ^= trf2[LS[91]];
											Columns[1] ^= LS[91];
										}

										Rows[9] ^= trf2[LS[90]];
										Columns[0] ^= LS[90];
									}
									Rows[5] ^= trf2[LS[54]];
									Columns[4] ^= LS[54];
									AD ^= LS[54];
									MD ^= trf[LS[54]];
								}
								Rows[4] ^= trf2[LS[44]];
								Columns[4] ^= LS[44];
								MD ^= LS[44];
								AD ^= trf[LS[44]];
							}
							Rows[6] ^= trf2[LS[63]];
							Columns[3] ^= LS[63];
							AD ^= LS[63];
							MD ^= trf[LS[63]];
						}
						Rows[3] ^= trf2[LS[33]];
						Columns[3] ^= LS[33];
						MD ^= LS[33];
						AD ^= trf[LS[33]];
					}
					Rows[7] ^= trf2[LS[72]];
					Columns[2] ^= LS[72];
					AD ^= LS[72];
					MD ^= trf[LS[72]];
				}
				Rows[2] ^= trf2[LS[22]];
				Columns[2] ^= LS[22];
				MD ^= LS[22];
				AD ^= trf[LS[22]];
			}
			Rows[8] ^= trf2[LS[81]];
			Columns[1] ^= LS[81];
			AD ^= LS[81];
			MD ^= trf[LS[81]];
		}
		Rows[1] ^= trf2[LS[11]];
		Columns[1] ^= LS[11];
		MD ^= LS[11];
		AD ^= trf[LS[11]];
	}
	return RES;
}

vector<vector<int>> TR2 = {
	{ 0,0,0,0,0 },
	{ 0,0,0,0,1 },{ 0,0,0,1,0 },{ 0,0,0,1,1 },{ 0,0,1,0,0 },
	{ 0,0,1,0,1 },{ 0,0,1,1,0 },{ 0,0,1,1,1 },{ 0,1,0,0,0 },
	{ 0,1,0,0,1 },{ 0,1,0,1,0 },{ 0,1,0,1,1 },{ 0,1,1,0,0 },
	{ 0,1,1,0,1 },{ 0,1,1,1,0 },{ 0,1,1,1,1 },{ 1,0,0,0,0 },
	{ 1,0,0,0,1 },{ 1,0,0,1,0 },{ 1,0,0,1,1 },{ 1,0,1,0,0 },
	{ 1,0,1,0,1 },{ 1,0,1,1,0 },{ 1,0,1,1,1 },{ 1,1,0,0,0 },
	{ 1,1,0,0,1 },{ 1,1,0,1,0 },{ 1,1,0,1,1 },{ 1,1,1,0,0 },
	{ 1,1,1,0,1 },{ 1,1,1,1,0 },{ 1,1,1,1,1 }
};

vector<vector<int>> TR4 = {
	{ 1,2,3,4 },{ 1,2,4,3 },{ 1,3,2,4 },{ 1,3,4,2 },
	{ 1,4,2,3 },{ 1,4,3,2 },{ 2,1,3,4 },{ 2,1,4,3 },
	{ 2,3,1,4 },{ 2,3,4,1 },{ 2,4,1,3 },{ 2,4,3,1 },
	{ 3,1,2,4 },{ 3,1,4,2 },{ 3,2,1,4 },{ 3,2,4,1 },
	{ 3,4,1,2 },{ 3,4,2,1 },{ 4,1,2,3 },{ 4,1,3,2 },
	{ 4,2,1,3 },{ 4,2,3,1 },{ 4,3,1,2 },{ 4,3,2,1 }
};
uint64_t Generate(vector<int> WU);

inline void print_sample(vector<int> LSM) {
	for (int i = 0; i < N / 2; i++) {
		cout << LSM[i] << " ";
	}
	for (int i = 0; i <N / 2; i++) {
		cout << (N - 1) - LSM[N / 2 - 1 - i] << " ";
	}
	cout << endl;
	int k = N;
	cout << "  " << LSM[N / 2 + 0] << "             " << N - 1 - LSM[N / 2 + 0] << endl;
	cout << "    " << LSM[N / 2 + 1] << "         " << N - 1 - LSM[N / 2 + 1] << endl;
	cout << "      " << LSM[N / 2 + 2] << "     " << N - 1 - LSM[N / 2 + 2] << endl;
	cout << "        " << LSM[N / 2 + 3] << " " << N - 1 - LSM[N / 2 + 3] << endl;
	cout << "        " << LSM[N / 2 + 4] << " " << N - 1 - LSM[N / 2 + 4] << endl;
	cout << "      " << LSM[N / 2 + 5] << "     " << N - 1 - LSM[N / 2 + 5] << endl;
	cout << "    " << LSM[N / 2 + 6] << "         " << N - 1 - LSM[N / 2 + 6] << endl;
	cout << "  " << LSM[N / 2 + 7] << "             " << N - 1 - LSM[N / 2 + 7] << endl;
	for (int i = 0; i < N / 2; i++) {
		cout << LSM[N / 2 + 8 + i] << " ";
	}
	for (int i = 0; i <N / 2; i++) {
		cout << (N - 1) - LSM[N + 8 - 1 - i] << " ";
	}
	cout << endl;
	cout << endl;
}

inline void flip_horizontal(vector<int> & LSM) {
	vector<int> t(LSM);
	for (int i = 0; i < N / 2; i++) {
		t[i] = LSM[N / 2 + 8 + i];
		t[N / 2 + 8 + i] = LSM[i];
	}
	t[N / 2] = LSM[N / 2 + 7];
	t[N / 2 + 1] = LSM[N / 2 + 6];
	t[N / 2 + 2] = LSM[N / 2 + 5];
	t[N / 2 + 3] = LSM[N / 2 + 4];
	t[N / 2 + 4] = LSM[N / 2 + 3];
	t[N / 2 + 5] = LSM[N / 2 + 2];
	t[N / 2 + 6] = LSM[N / 2 + 1];
	t[N / 2 + 7] = LSM[N / 2 + 0];
	LSM = t;
}

inline void flip_vertical(vector<int> & LSM) {
	for (int i = 0; i < LSM.size(); i++) {
		LSM[i] = N - 1 - LSM[i];
	}
}


inline void normalize(vector<int> & LSM) {
	vector<int> row0;
	for (int i = 0; i < N / 2; i++) {
		row0.push_back(LSM[i]);
	}
	for (int i = 0; i <N / 2; i++) {
		row0.push_back((N - 1) - LSM[N / 2 - 1 - i]);
	}
	vector<int> subst(N);
	for (int i = 0; i < N; i++) {
		subst[row0[i]] = i;
	}
	for (int i = 0; i < LSM.size(); i++) {
		LSM[i] = subst[LSM[i]];
	}
}

static inline void Transform2(vector<int> & LSM, vector<int> tr2) {
	if (tr2[0] == 1) {
		for (int i = 0; i < N / 2; i++) {
			swap(LSM[i], LSM[N / 2 + 8 + i]);
		}
		LSM[0] = (N - 1) - LSM[0];
		LSM[N / 2 + 8 + 0] = (N - 1) - LSM[N / 2 + 8 + 0];
	}
	//print_sample(LSM);
	if (tr2[1] == 1) {
		swap(LSM[N / 2], LSM[N / 2 + 7]);
		LSM[N / 2] = N - 1 - LSM[N / 2];
		LSM[N / 2 + 7] = N - 1 - LSM[N / 2 + 7];
		LSM[1] = (N - 1) - LSM[1];
		LSM[N / 2 + 8 + 1] = (N - 1) - LSM[N / 2 + 8 + 1];
	}
	//print_sample(LSM);
	if (tr2[2] == 1) {
		swap(LSM[N / 2 + 1], LSM[N / 2 + 6]);
		LSM[N / 2 + 1] = N - 1 - LSM[N / 2 + 1];
		LSM[N / 2 + 6] = N - 1 - LSM[N / 2 + 6];
		LSM[2] = (N - 1) - LSM[2];
		LSM[N / 2 + 8 + 2] = (N - 1) - LSM[N / 2 + 8 + 2];
	}
	//print_sample(LSM);
	if (tr2[3] == 1) {
		swap(LSM[N / 2 + 2], LSM[N / 2 + 5]);
		LSM[N / 2 + 2] = N - 1 - LSM[N / 2 + 2];
		LSM[N / 2 + 5] = N - 1 - LSM[N / 2 + 5];
		LSM[3] = (N - 1) - LSM[3];
		LSM[N / 2 + 8 + 3] = (N - 1) - LSM[N / 2 + 8 + 3];
	}
	//print_sample(LSM);
	if (tr2[4] == 1) {
		swap(LSM[N / 2 + 3], LSM[N / 2 + 4]);
		LSM[N / 2 + 3] = N - 1 - LSM[N / 2 + 3];
		LSM[N / 2 + 4] = N - 1 - LSM[N / 2 + 4];
		LSM[4] = (N - 1) - LSM[4];
		LSM[N / 2 + 8 + 4] = (N - 1) - LSM[N / 2 + 8 + 4];
	}
	//print_sample(LSM);
}

static inline void Transform4(vector<int> & LSM, vector<int> tr4) {
	//{ 2,1,3,4 }
	//0
	vector<int> t(LSM);
	for (int i = 0; i < tr4.size(); i++) {
		t[i + 1] = LSM[tr4[i]];
		t[N / 2 + 8 + i + 1] = LSM[N / 2 + 8 + tr4[i]];

		t[N / 2 + i] = LSM[N / 2 + tr4[i] - 1];
		t[N / 2 + 8 - i - 1] = LSM[N / 2 + 8 - tr4[i]];
	}
	LSM = t;
	//print_sample(LSM);
}


int* trf3 = new int[1 << N];


static inline vector<int> WUtoLSM(vector<int> WU) {
	vector<int> LSM;
	for (int i = 0; i < N / 2; i++) {
		LSM.push_back(i);
	}

	for (int i = 0; i < WU.size(); i++) {
		LSM.push_back(trf3[WU[i]]);
	}
	return LSM;
}
static inline vector<int> LSMtoWU(vector<int> LSM) {
	vector<int> WU;
	for (int i = N / 2; i < LSM.size(); i++) {
		WU.push_back(1 << LSM[i]);
	}
	return WU;
}

void printvec(vector<int> sr) {
	for (int i = 0; i < sr.size(); i++) {
		cout << sr[i] << " ";
	}
	cout << endl;
}

void PROCESS_WU(std::map<vector<int>, int> &WUMAP_CHECKED, map<vector<int>, int> &WU_TOCHECK) {

	//vector<vector<int>> ST;
	for (auto &ent1 : WUMAP_CHECKED) {
		if (ent1.second == 0) {
			vector<int> input = ent1.first;
			set<vector<int>> ST;
			vector<int> LSM = WUtoLSM(input);

			for (int i = 0; i < TR2.size(); i++) {
				for (int j = 0; j < TR4.size(); j++) {
					vector<int> t(LSM);
					Transform2(t, TR2[i]);
					Transform4(t, TR4[j]);
					vector<int> th(t);
					flip_horizontal(th);
					vector<int> tv(t);
					flip_vertical(tv);
					vector<int> thv(th);
					flip_vertical(thv);

					normalize(t);
					normalize(th);
					normalize(tv);
					normalize(thv);

					ST.insert(t);
					ST.insert(th);
					//			ST.insert(tv);
					//		ST.insert(thv);
				}
			}

			for (auto ent2 : ST) {
				vector<int> p = LSMtoWU(ent2);
				WUMAP_CHECKED[p] = 1;
			}
			vector<int> rep = *ST.begin();
			vector<int> p_rep = LSMtoWU(rep);
			WU_TOCHECK[p_rep] = ST.size();
		}
	}

	/*	uint64_t orig_cnt = Generate(input);
	cout << "Original WU:" << endl;
	printvec(input);
	cout << "Original WU count : " << orig_cnt << endl;

	cout << "ST size " << ST.size() << endl;
	for (auto f : ST) {
	vector<int> twu = LSMtoWU(f);
	uint64_t tmp_cnt = Generate(twu);
	if (tmp_cnt != orig_cnt) {
	cout << "ALARM!!!\n";
	getchar();
	}
	}
	*/
	//getchar();
	/*
	print_sample(LSM);

	vector<int> TMP(LSM);
	flip_horizontal(TMP);
	print_sample(TMP);

	normalize(TMP);
	print_sample(TMP);
	cout << "Transform 2 {1,1,1,1,1}" << endl;
	Transform2(TMP, { 1,1,1,1,1 });
	print_sample(TMP);


	cout << "Transform 4 {4,3,2,1}" << endl;
	Transform4(TMP, { 4,3,2,1 });
	print_sample(TMP);
	*/
	//	getchar();
	//LSMASK ready. First 5 - first row, last 5 - last row. Middle - diagonal
	/*
	Viable transformations:
	0-th type
	mirroring
	Main diagonal
	Main antidiagonal
	horizontally
	vertically

	1st type:
	0-th and 9th row
	1-st and 8th row
	2-nd and 7th row
	3-rd and 6th row
	4-th and 5th row

	2^5
	2nd type
	1st and 2nd row + 7-th and 8-th row
	1st and 3rd row + 6-th and 8-th row
	1st and 4th row + 5-th and 8-th row

	2nd and 3rd row + 6-th and 7-th row
	2nd and 4th row + 5-th and 7-th row

	3rd and 4th row + 5-th and 6-th row
	*/

}



void main()
{
	for (int i = 0; i < N; i++) { trf3[1 << i] = i; }
	//test
	//vector<int> tWU = { 4, 2, 16, 8, 4, 2, 8, 1, 16, 8, 1, 4, 2 };
	//PROCESS_WU(tWU);

	//


	vector<vector<int>> WUS = Generate_WU();
	cout << "WU vector size :" << WUS.size() << endl;
	//getchar();
	//int* trf3 = new int[1 << N];
	/*
	cout << "WU vector size :" << WUS.size() << endl;
	cout << "Test:\n";
	for (int i = 0; i < WUS[0].size(); i++) {
	cout << WUS[0][i] << " ";
	}
	cout << endl;

	vector<int> rr = WUtoLSM(WUS[0]);

	for (int i = 0; i < rr.size(); i++) {
	cout << rr[i]<< " ";
	}
	cout << endl;

	vector<int> rrb = LSMtoWU(rr);

	for (int i = 0; i < rrb.size(); i++) {
	cout << rrb[i] << " ";
	}
	cout << endl;

	getchar();*/

	std::map<vector<int>, int> WU_MAP_checked;

	for (int i = 0; i < WUS.size(); i++) {
		WU_MAP_checked[WUS[i]] = 0;
	}

	std::map<vector<int>, int> WU_MAP_tocheck;

	double t_c0 = cpuTime();
	PROCESS_WU(WU_MAP_checked, WU_MAP_tocheck);
	double t_c1 = cpuTime();

	cout << "Filtering took " << t_c1 - t_c0 << endl;

	cout << "WU_MAP_CHECKED SIZE : " << WU_MAP_checked.size() << endl;

	cout << "WU_MAP_TOCHECK SIZE : " << WU_MAP_tocheck.size() << endl;

	vector<vector<int>> TC;

	for (auto ent0 : WU_MAP_tocheck) {
		TC.push_back(ent0.first);
	}

	vector<uint64_t> EXP_RES(TC.size());

	double tt0 = cpuTime();
#pragma omp parallel for
	for (int i = 0; i < TC.size(); i++) {
		EXP_RES[i] = Generate(TC[i]);
		if ((cur_cnt  > 0)&(cur_cnt % 200 == 0)) {
			cout << "Current progress: " << cur_cnt << "out of " << TC.size() << endl;
		}
	}
	double ttf = cpuTime();
	cout << "Experiment took " << ttf - tt0 << endl;

	uint64_t SDLS10_cnt = 0;
	for (int i = 0; i < TC.size(); i++) {
		SDLS10_cnt += EXP_RES[i] * WU_MAP_tocheck[TC[i]];
	}
	cout << "SDLS 10 count : " << SDLS10_cnt << endl;


	ofstream out;
	out.open("D:\\LSTests\\SDLS10.log");
	for (int i = 0; i < TC.size(); i++) {
		for (int j = 0; j < TC[i].size(); j++) {
			out << trf3[TC[i][j]] << " ";
		}
		out << ": " << EXP_RES[i] << " [" << WU_MAP_tocheck[TC[i]] << "]" << endl;
	}
	out.close();


	getchar();


	/*
	uint64_t SDLS10_cnt= 0;
	int k = 0;
	double t0 = cpuTime();
	for (auto ent0 : WU_MAP_tocheck) {
	uint64_t t_cnt = Generate(ent0.first);
	SDLS10_cnt += t_cnt*(uint64_t)ent0.second;
	k++;
	if ((k > 0) && (k % 100 == 0)) {
	cout << "Processed " << k << " out of " << WU_MAP_tocheck.size() << endl;
	double t1 = cpuTime();
	cout << "Time spent: " << t1 - t0 << endl;
	double estim = ((t1 - t0) / (double)k) *(double) WU_MAP_tocheck.size();
	cout << "Estimated time: " << estim << " (" << estim - (t1 - t0) << " to go)" << endl;
	}

	}
	double tf = cpuTime();
	*/
	cout << "SDLS 10 count : " << SDLS10_cnt << endl;
	//cout << "Time spent : " << tf-t0<< endl;

	getchar();

	/*



	for (int i = 0; i < WUS.size(); i++) {
	double t1 = cpuTime();
	//Reset();
	//PROCESS_WU(WUS[i]);
	uint64_t S_cnt = Generate(WUS[i]);
	double t2 = cpuTime();
	cout << "Workunit " << i << ":\n";
	for (int u = 0; u < WUS[i].size(); u++) {
	cout << trf3[WUS[i][u]] << " ";
	}
	cout << endl;
	cout << "Workunit squares count: " << S_cnt << endl;
	cout << "Time spent: " << t2 - t1 << endl;

	}*/

	cout << "OK";
	getchar();
}
uint64_t Generate(vector<int> WU)
{
	cur_cnt++;
	uint64_t SquaresCnt = 0;
	vector<vector<int>> RES;
	int32_t LS[N*N];
	int32_t CR[N*N];
	uint32_t Columns[N], Rows[N], MD, AD;
	int* trf = new int[1 << N];
	int* trf2 = new int[1 << N];

	for (int i = 0; i < N; i++) { trf[1 << i] = 1 << (N - 1 - i); }
	for (int i = 0; i < N; i++) { trf2[1 << i] = (1 << i) | (1 << (N - 1 - i)); }

	for (int i = 0; i < N*N; i++)
		LS[i] = 0;
	for (int i = 0; i < N; i++)
		LS[i] = 1 << i;
	for (int i = 0; i < N; i++)
		Columns[i] = 0;
	for (int i = 0; i < N; i++)
		Rows[i] = 0;
	MD = 0;
	AD = 0;
	for (int j = 0; j<N; j++) {
		LS[j] = 1 << j;
		Rows[0] |= LS[j];
		Columns[j] |= LS[j];
	}
	MD |= LS[0];
	AD |= LS[N - 1];

	uint64_t k = 0;
	double t0 = cpuTime();
	//vector<int32_t> t = { LS[11],LS[22],LS[33],LS[44],LS[54],LS[63],LS[72], LS[81], LS[90], LS[91], LS[92], LS[93], LS[94] };
	//for (CR[11] = AllN ^ (Rows[1] | Columns[1] | MD); CR[11] != 0; CR[11] = CR[11] = (CR[11] & (CR[11] - 1))) {
	//LS[11] = (CR[11] & (-CR[11]));
	{
		LS[11] = WU[0];
		Rows[1] |= trf2[LS[11]];
		Columns[1] |= LS[11];
		MD |= LS[11];
		AD |= trf[LS[11]];
		//for (CR[81] = AllN ^ (Rows[8] | Columns[1] | AD); CR[81] != 0; CR[81] = CR[81] = (CR[81] & (CR[81] - 1))) {
		//LS[81] = (CR[81] & (-CR[81]));
		{
			LS[81] = WU[7];
			Rows[8] |= trf2[LS[81]];
			Columns[1] |= LS[81];
			AD |= LS[81];
			MD |= trf[LS[81]];
			//for (CR[22] = AllN ^ (Rows[2] | Columns[2] | MD); CR[22] != 0; CR[22] = CR[22] = (CR[22] & (CR[22] - 1))) {
			//LS[22] = (CR[22] & (-CR[22]));
			{
				LS[22] = WU[1];
				Rows[2] |= trf2[LS[22]];
				Columns[2] |= LS[22];
				MD |= LS[22];
				AD |= trf[LS[22]];
				//for (CR[72] = AllN ^ (Rows[7] | Columns[2] | AD); CR[72] != 0; CR[72] = CR[72] = (CR[72] & (CR[72] - 1))) {
				{
					//	LS[72] = (CR[72] & (-CR[72]));
					LS[72] = WU[6];
					Rows[7] |= trf2[LS[72]];
					Columns[2] |= LS[72];
					AD |= LS[72];
					MD |= trf[LS[72]];
					//for (CR[33] = AllN ^ (Rows[3] | Columns[3] | MD); CR[33] != 0; CR[33] = CR[33] = (CR[33] & (CR[33] - 1))) {
					//LS[33] = (CR[33] & (-CR[33]));
					{
						LS[33] = WU[2];
						Rows[3] |= trf2[LS[33]];
						Columns[3] |= LS[33];
						MD |= LS[33];
						AD |= trf[LS[33]];
						//for (CR[63] = AllN ^ (Rows[6] | Columns[3] | AD); CR[63] != 0; CR[63] = CR[63] = (CR[63] & (CR[63] - 1))) {
						//LS[63] = (CR[63] & (-CR[63]));
						{
							LS[63] = WU[5];
							Rows[6] |= trf2[LS[63]];
							Columns[3] |= LS[63];
							AD |= LS[63];
							MD |= trf[LS[63]];
							//for (CR[44] = AllN ^ (Rows[4] | Columns[4] | MD); CR[44] != 0; CR[44] = CR[44] = (CR[44] & (CR[44] - 1))) {
							//LS[44] = (CR[44] & (-CR[44]));
							{
								LS[44] = WU[3];
								Rows[4] |= trf2[LS[44]];
								Columns[4] |= LS[44];
								MD |= LS[44];
								AD |= trf[LS[44]];
								//for (CR[54] = AllN ^ (Rows[5] | Columns[4] | AD); CR[54] != 0; CR[54] = CR[54] = (CR[54] & (CR[54] - 1))) {
								//LS[54] = (CR[54] & (-CR[54]));
								{
									LS[54] = WU[4];
									Rows[5] |= trf2[LS[54]];
									Columns[4] |= LS[54];
									AD |= LS[54];
									MD |= trf[LS[54]];
									CR[90] = AllN ^ (Rows[9] | Columns[0] | AD);
									//if (CR[90] != 0) {

									//LS[90] = (CR[90] & (-CR[90]));
									{
										LS[90] = WU[8];
										Rows[9] |= trf2[LS[90]];
										Columns[0] |= LS[90];
										//81088
										//	for (CR[91] = AllN ^ (Rows[9] | Columns[1]); CR[91] != 0; CR[91] = CR[91] = (CR[91] & (CR[91] - 1))) {
										//	LS[91] = (CR[91] & (-CR[91]));
										{
											LS[91] = WU[9];
											Rows[9] |= trf2[LS[91]];
											Columns[1] |= LS[91];
											//for (CR[92] = AllN ^ (Rows[9] | Columns[2]); CR[92] != 0; CR[92] = CR[92] = (CR[92] & (CR[92] - 1))) {
											//LS[92] = (CR[92] & (-CR[92]));
											{
												LS[92] = WU[10];
												Rows[9] |= trf2[LS[92]];
												Columns[2] |= LS[92];
												//for (CR[93] = AllN ^ (Rows[9] | Columns[3]); CR[93] != 0; CR[93] = CR[93] = (CR[93] & (CR[93] - 1))) {
												//LS[93] = (CR[93] & (-CR[93]));
												{
													LS[93] = WU[11];
													Rows[9] |= trf2[LS[93]];
													Columns[3] |= LS[93];
													//for (CR[94] = AllN ^ (Rows[9] | Columns[4]); CR[94] != 0; CR[94] = CR[94] = (CR[94] & (CR[94] - 1))) {
													//LS[94] = (CR[94] & (-CR[94]));
													{
														LS[94] = WU[12];
														Rows[9] |= trf2[LS[94]];
														Columns[4] |= LS[94];
														//7207872

														for (CR[12] = AllN ^ (Rows[1] | Columns[2]); CR[12] != 0; CR[12] = CR[12] = (CR[12] & (CR[12] - 1))) {
															LS[12] = (CR[12] & (-CR[12]));
															Rows[1] |= trf2[LS[12]];
															Columns[2] |= LS[12];
															for (CR[13] = AllN ^ (Rows[1] | Columns[3]); CR[13] != 0; CR[13] = CR[13] = (CR[13] & (CR[13] - 1))) {
																LS[13] = (CR[13] & (-CR[13]));
																Rows[1] |= trf2[LS[13]];
																Columns[3] |= LS[13];
																for (CR[14] = AllN ^ (Rows[1] | Columns[4]); CR[14] != 0; CR[14] = CR[14] = (CR[14] & (CR[14] - 1))) {
																	LS[14] = (CR[14] & (-CR[14]));
																	Rows[1] |= trf2[LS[14]];
																	Columns[4] |= LS[14];
																	for (CR[10] = AllN ^ (Rows[1] | Columns[0]); CR[10] != 0; CR[10] = CR[10] = (CR[10] & (CR[10] - 1))) {
																		LS[10] = (CR[10] & (-CR[10]));
																		Rows[1] |= trf2[LS[10]];
																		Columns[0] |= LS[10];
																		for (CR[23] = AllN ^ (Rows[2] | Columns[3]); CR[23] != 0; CR[23] = CR[23] = (CR[23] & (CR[23] - 1))) {
																			LS[23] = (CR[23] & (-CR[23]));
																			Rows[2] |= trf2[LS[23]];
																			Columns[3] |= LS[23];
																			for (CR[24] = AllN ^ (Rows[2] | Columns[4]); CR[24] != 0; CR[24] = CR[24] = (CR[24] & (CR[24] - 1))) {
																				LS[24] = (CR[24] & (-CR[24]));
																				Rows[2] |= trf2[LS[24]];
																				Columns[4] |= LS[24];
																				for (CR[21] = AllN ^ (Rows[2] | Columns[1]); CR[21] != 0; CR[21] = CR[21] = (CR[21] & (CR[21] - 1))) {
																					LS[21] = (CR[21] & (-CR[21]));
																					Rows[2] |= trf2[LS[21]];
																					Columns[1] |= LS[21];
																					for (CR[20] = AllN ^ (Rows[2] | Columns[0]); CR[20] != 0; CR[20] = CR[20] = (CR[20] & (CR[20] - 1))) {
																						LS[20] = (CR[20] & (-CR[20]));
																						Rows[2] |= trf2[LS[20]];
																						Columns[0] |= LS[20];
																						for (CR[34] = AllN ^ (Rows[3] | Columns[4]); CR[34] != 0; CR[34] = CR[34] = (CR[34] & (CR[34] - 1))) {
																							LS[34] = (CR[34] & (-CR[34]));
																							Rows[3] |= trf2[LS[34]];
																							Columns[4] |= LS[34];
																							for (CR[31] = AllN ^ (Rows[3] | Columns[1]); CR[31] != 0; CR[31] = CR[31] = (CR[31] & (CR[31] - 1))) {
																								LS[31] = (CR[31] & (-CR[31]));
																								Rows[3] |= trf2[LS[31]];
																								Columns[1] |= LS[31];
																								for (CR[32] = AllN ^ (Rows[3] | Columns[2]); CR[32] != 0; CR[32] = CR[32] = (CR[32] & (CR[32] - 1))) {
																									LS[32] = (CR[32] & (-CR[32]));
																									Rows[3] |= trf2[LS[32]];
																									Columns[2] |= LS[32];
																									for (CR[30] = AllN ^ (Rows[3] | Columns[0]); CR[30] != 0; CR[30] = CR[30] = (CR[30] & (CR[30] - 1))) {
																										LS[30] = (CR[30] & (-CR[30]));
																										Rows[3] |= trf2[LS[30]];
																										Columns[0] |= LS[30];
																										for (CR[64] = AllN ^ (Rows[6] | Columns[4]); CR[64] != 0; CR[64] = CR[64] = (CR[64] & (CR[64] - 1))) {
																											LS[64] = (CR[64] & (-CR[64]));
																											Rows[6] |= trf2[LS[64]];
																											Columns[4] |= LS[64];
																											for (CR[61] = AllN ^ (Rows[6] | Columns[1]); CR[61] != 0; CR[61] = CR[61] = (CR[61] & (CR[61] - 1))) {
																												LS[61] = (CR[61] & (-CR[61]));
																												Rows[6] |= trf2[LS[61]];
																												Columns[1] |= LS[61];
																												for (CR[62] = AllN ^ (Rows[6] | Columns[2]); CR[62] != 0; CR[62] = CR[62] = (CR[62] & (CR[62] - 1))) {
																													LS[62] = (CR[62] & (-CR[62]));
																													Rows[6] |= trf2[LS[62]];
																													Columns[2] |= LS[62];
																													for (CR[60] = AllN ^ (Rows[6] | Columns[0]); CR[60] != 0; CR[60] = CR[60] = (CR[60] & (CR[60] - 1))) {
																														LS[60] = (CR[60] & (-CR[60]));
																														Rows[6] |= trf2[LS[60]];
																														Columns[0] |= LS[60];
																														for (CR[74] = AllN ^ (Rows[7] | Columns[4]); CR[74] != 0; CR[74] = CR[74] = (CR[74] & (CR[74] - 1))) {
																															LS[74] = (CR[74] & (-CR[74]));
																															Rows[7] |= trf2[LS[74]];
																															Columns[4] |= LS[74];
																															CR[84] = AllN ^ (Rows[8] | Columns[4]);
																															if (CR[84] != 0) {
																																LS[84] = (CR[84] & (-CR[84]));
																																Rows[8] |= trf2[LS[84]];
																																for (CR[71] = AllN ^ (Rows[7] | Columns[1]); CR[71] != 0; CR[71] = CR[71] = (CR[71] & (CR[71] - 1))) {
																																	LS[71] = (CR[71] & (-CR[71]));
																																	Rows[7] |= trf2[LS[71]];
																																	Columns[1] |= LS[71];
																																	for (CR[70] = AllN ^ (Rows[7] | Columns[0]); CR[70] != 0; CR[70] = CR[70] = (CR[70] & (CR[70] - 1))) {
																																		LS[70] = (CR[70] & (-CR[70]));
																																		Rows[7] |= trf2[LS[70]];
																																		Columns[0] |= LS[70];
																																		for (CR[73] = AllN ^ (Rows[7] | Columns[3]); CR[73] != 0; CR[73] = CR[73] = (CR[73] & (CR[73] - 1))) {
																																			LS[73] = (CR[73] & (-CR[73]));
																																			Rows[7] |= trf2[LS[73]];
																																			Columns[3] |= LS[73];
																																			for (CR[80] = AllN ^ (Rows[8] | Columns[0]); CR[80] != 0; CR[80] = CR[80] = (CR[80] & (CR[80] - 1))) {
																																				LS[80] = (CR[80] & (-CR[80]));
																																				Rows[8] |= trf2[LS[80]];
																																				Columns[0] |= LS[80];
																																				for (CR[82] = AllN ^ (Rows[8] | Columns[2]); CR[82] != 0; CR[82] = CR[82] = (CR[82] & (CR[82] - 1))) {
																																					LS[82] = (CR[82] & (-CR[82]));
																																					Rows[8] |= trf2[LS[82]];
																																					Columns[2] |= LS[82];
																																					for (CR[83] = AllN ^ (Rows[8] | Columns[3]); CR[83] != 0; CR[83] = CR[83] = (CR[83] & (CR[83] - 1))) {
																																						LS[83] = (CR[83] & (-CR[83]));
																																						Rows[8] |= trf2[LS[83]];
																																						Columns[3] |= LS[83];
																																						for (CR[40] = AllN ^ (Rows[4] | Columns[0]); CR[40] != 0; CR[40] = CR[40] = (CR[40] & (CR[40] - 1))) {
																																							LS[40] = (CR[40] & (-CR[40]));
																																							Rows[4] |= trf2[LS[40]];
																																							Columns[0] |= LS[40];
																																							CR[50] = AllN ^ (Rows[5] | Columns[0]);
																																							if (CR[50] != 0) {
																																								LS[50] = (CR[50] & (-CR[50]));
																																								Rows[5] |= trf2[LS[50]];
																																								for (CR[41] = AllN ^ (Rows[4] | Columns[1]); CR[41] != 0; CR[41] = CR[41] = (CR[41] & (CR[41] - 1))) {
																																									LS[41] = (CR[41] & (-CR[41]));
																																									Rows[4] |= trf2[LS[41]];
																																									Columns[1] |= LS[41];
																																									CR[51] = AllN ^ (Rows[5] | Columns[1]);
																																									if (CR[51] != 0) {
																																										LS[51] = (CR[51] & (-CR[51]));
																																										Rows[5] |= trf2[LS[51]];
																																										for (CR[42] = AllN ^ (Rows[4] | Columns[2]); CR[42] != 0; CR[42] = CR[42] = (CR[42] & (CR[42] - 1))) {
																																											LS[42] = (CR[42] & (-CR[42]));
																																											Rows[4] |= trf2[LS[42]];
																																											Columns[2] |= LS[42];
																																											CR[52] = AllN ^ (Rows[5] | Columns[2]);
																																											if (CR[52] != 0) {
																																												LS[52] = (CR[52] & (-CR[52]));
																																												Rows[5] |= trf2[LS[52]];
																																												for (CR[43] = AllN ^ (Rows[4] | Columns[3]); CR[43] != 0; CR[43] = CR[43] = (CR[43] & (CR[43] - 1))) {
																																													LS[43] = (CR[43] & (-CR[43]));
																																													Rows[4] |= trf2[LS[43]];
																																													Columns[3] |= LS[43];
																																													CR[53] = AllN ^ (Rows[5] | Columns[3]);
																																													if (CR[53] != 0) {
																																														LS[53] = (CR[53] & (-CR[53]));
																																														Rows[5] |= trf2[LS[53]];
																																														SquaresCnt++;
																																														if (SquaresCnt % 100000000 == 0) {
																																															cout << "Squares found: " << SquaresCnt << endl;
																																														}
																																														Rows[5] ^= trf2[LS[53]];
																																													}
																																													Rows[4] ^= trf2[LS[43]];
																																													Columns[3] ^= LS[43];
																																												}
																																												Rows[5] ^= trf2[LS[52]];
																																											}
																																											Rows[4] ^= trf2[LS[42]];
																																											Columns[2] ^= LS[42];
																																										}
																																										Rows[5] ^= trf2[LS[51]];
																																									}
																																									Rows[4] ^= trf2[LS[41]];
																																									Columns[1] ^= LS[41];
																																								}
																																								Rows[5] ^= trf2[LS[50]];
																																							}
																																							Rows[4] ^= trf2[LS[40]];
																																							Columns[0] ^= LS[40];
																																						}
																																						Rows[8] ^= trf2[LS[83]];
																																						Columns[3] ^= LS[83];
																																					}
																																					Rows[8] ^= trf2[LS[82]];
																																					Columns[2] ^= LS[82];
																																				}
																																				Rows[8] ^= trf2[LS[80]];
																																				Columns[0] ^= LS[80];
																																			}
																																			Rows[7] ^= trf2[LS[73]];
																																			Columns[3] ^= LS[73];
																																		}
																																		Rows[7] ^= trf2[LS[70]];
																																		Columns[0] ^= LS[70];
																																	}
																																	Rows[7] ^= trf2[LS[71]];
																																	Columns[1] ^= LS[71];
																																}
																																Rows[8] ^= trf2[LS[84]];
																															}
																															Rows[7] ^= trf2[LS[74]];
																															Columns[4] ^= LS[74];
																														}
																														Rows[6] ^= trf2[LS[60]];
																														Columns[0] ^= LS[60];
																													}
																													Rows[6] ^= trf2[LS[62]];
																													Columns[2] ^= LS[62];
																												}
																												Rows[6] ^= trf2[LS[61]];
																												Columns[1] ^= LS[61];
																											}
																											Rows[6] ^= trf2[LS[64]];
																											Columns[4] ^= LS[64];
																										}
																										Rows[3] ^= trf2[LS[30]];
																										Columns[0] ^= LS[30];
																									}
																									Rows[3] ^= trf2[LS[32]];
																									Columns[2] ^= LS[32];
																								}
																								Rows[3] ^= trf2[LS[31]];
																								Columns[1] ^= LS[31];
																							}
																							Rows[3] ^= trf2[LS[34]];
																							Columns[4] ^= LS[34];
																						}
																						Rows[2] ^= trf2[LS[20]];
																						Columns[0] ^= LS[20];
																					}
																					Rows[2] ^= trf2[LS[21]];
																					Columns[1] ^= LS[21];
																				}
																				Rows[2] ^= trf2[LS[24]];
																				Columns[4] ^= LS[24];
																			}
																			Rows[2] ^= trf2[LS[23]];
																			Columns[3] ^= LS[23];
																		}
																		Rows[1] ^= trf2[LS[10]];
																		Columns[0] ^= LS[10];
																	}
																	Rows[1] ^= trf2[LS[14]];
																	Columns[4] ^= LS[14];
																}
																Rows[1] ^= trf2[LS[13]];
																Columns[3] ^= LS[13];
															}
															Rows[1] ^= trf2[LS[12]];
															Columns[2] ^= LS[12];
														}

														Rows[9] ^= trf2[LS[94]];
														Columns[4] ^= LS[94];
													}
													Rows[9] ^= trf2[LS[93]];
													Columns[3] ^= LS[93];
												}
												Rows[9] ^= trf2[LS[92]];
												Columns[2] ^= LS[92];
											}
											Rows[9] ^= trf2[LS[91]];
											Columns[1] ^= LS[91];
										}

										Rows[9] ^= trf2[LS[90]];
										Columns[0] ^= LS[90];
									}
									Rows[5] ^= trf2[LS[54]];
									Columns[4] ^= LS[54];
									AD ^= LS[54];
									MD ^= trf[LS[54]];
								}
								Rows[4] ^= trf2[LS[44]];
								Columns[4] ^= LS[44];
								MD ^= LS[44];
								AD ^= trf[LS[44]];
							}
							Rows[6] ^= trf2[LS[63]];
							Columns[3] ^= LS[63];
							AD ^= LS[63];
							MD ^= trf[LS[63]];
						}
						Rows[3] ^= trf2[LS[33]];
						Columns[3] ^= LS[33];
						MD ^= LS[33];
						AD ^= trf[LS[33]];
					}
					Rows[7] ^= trf2[LS[72]];
					Columns[2] ^= LS[72];
					AD ^= LS[72];
					MD ^= trf[LS[72]];
				}
				Rows[2] ^= trf2[LS[22]];
				Columns[2] ^= LS[22];
				MD ^= LS[22];
				AD ^= trf[LS[22]];
			}
			Rows[8] ^= trf2[LS[81]];
			Columns[1] ^= LS[81];
			AD ^= LS[81];
			MD ^= trf[LS[81]];
		}
		Rows[1] ^= trf2[LS[11]];
		Columns[1] ^= LS[11];
		MD ^= LS[11];
		AD ^= trf[LS[11]];
	}
	return SquaresCnt;
}
