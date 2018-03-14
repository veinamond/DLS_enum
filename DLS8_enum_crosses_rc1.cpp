#include <iostream>
#include <chrono>
#include <vector>
#include <set>

static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
using namespace std;
const int N = 8;
const uint32_t MaxN = 1 << N;
const uint32_t AllN = MaxN - 1;
int32_t LS[N*N];
int32_t CR[N*N];
int32_t L[N*N];
int32_t CNT[N*N];
uint64_t SquaresCnt = 0;
uint32_t Columns[N], Rows[N], MD, AD;
int* trf3 = new int[1 << N];

vector<vector<int>> TR2 = {
	{ 0,0,0,0 },{ 1,0,0,0 },{ 0,1,0,0 },{ 1,1,0,0 },
	{ 0,0,1,0 },{ 1,0,1,0 },{ 0,1,1,0 },{ 1,1,1,0 },
	{ 0,0,0,1 },{ 1,0,0,1 },{ 0,1,0,1 },{ 1,1,0,1 },
	{ 0,0,1,1 },{ 1,0,1,1 },{ 0,1,1,1 },{ 1,1,1,1 }
};

vector<vector<int>> TR4 = {
	{ 1,2,3 },{ 1,3,2 },{ 2,1,3 },{ 2,3,1 },{ 3,1,2 },{ 3,2,1 }
};
vector<vector<int>> TR4_cross = {
	{ 1,2,3,4 },{ 1,3,2,4 },{ 2,1,3,4 },{ 2,3,1,4 },{ 3,1,2,4 },{ 3,2,1,4 },
	{ 1,2,4,3 },{ 1,3,4,2 },{ 2,1,4,3 },{ 2,3,4,1 },{ 3,1,4,2 },{ 3,2,4,1 },
	{ 1,4,2,3 },{ 1,4,3,2 },{ 2,4,1,3 },{ 2,4,3,1 },{ 3,4,1,2 },{ 3,4,2,1 },
	{ 4,1,2,3 },{ 4,1,3,2 },{ 4,2,1,3 },{ 4,2,3,1 },{ 4,3,1,2 },{ 4,3,2,1 }
};

void print_cross(vector<int> LSC) {
	cout << trf3[LSC[0]] << "           " << trf3[LSC[N + 0]] << "\n";
	cout << "  " << trf3[LSC[1]] << "       " << trf3[LSC[N + 1]] << " " << "\n";
	cout << "    " << trf3[LSC[2]] << "   " << trf3[LSC[N + 2]] << "  " << "\n";
	cout << "     " << trf3[LSC[3]] << " " << trf3[LSC[N + 3]] << "   " << "\n";
	cout << "     " << trf3[LSC[N + 4]] << " " << trf3[LSC[4]] << "   " << "\n";
	cout << "    " << trf3[LSC[N + 5]] << "   " << trf3[LSC[5]] << "  " << "\n";
	cout << "  " << trf3[LSC[N + 6]] << "       " << trf3[LSC[6]] << " " << "\n";
	cout << trf3[LSC[N + 7]] << "           " << trf3[LSC[7]] << "\n";
}

static inline void Transpond(vector<int> & LSC) {
	/*cout << "before tr4\n";
	for (int i = 0; i < tr4.size(); i++) {
	cout << tr4[i] << " ";
	}
	cout << "\n";
	print_cross(LSC);*/
	swap(LSC[N], LSC[N + 7]);
	swap(LSC[N + 1], LSC[N + 6]);
	swap(LSC[N + 2], LSC[N + 5]);
	swap(LSC[N + 3], LSC[N + 4]);	

	//cout << "After tr4\n";
	//print_cross(LSC);
}

inline void flip_vertical_cross(vector<int> & LSC) {
	//debug

	/*cout << "Before vertical flip\n";
	print_cross(LSC);*/
	for (int i = 0; i < N; i++) {
		swap(LSC[i], LSC[N + i]);
	}
	/*
	cout << "After vertical flip\n";
	print_cross(LSC);
	*/
}

inline void flip_horizontal_cross(vector<int> & LSC) {
	/*cout << "Before horizontal flip\n";
	print_cross(LSC);*/
	for (int i = 0; i < N / 2; i++) {
		swap(LSC[i], LSC[2 * N - 1 - i]);
		swap(LSC[N + i], LSC[N - 1 - i]);
	}
	/*cout << "After horizontal flip\n";
	print_cross(LSC);*/
}


inline void normalize(vector<int> & LSM) {
	vector<int> subst(N);
	for (int i = 0; i < N; i++) {
		subst[trf3[LSM[i]]] = 1 << i;
	}
	for (int i = 0; i < LSM.size(); i++) {
		LSM[i] = subst[trf3[LSM[i]]];
	}
}

static inline void Transform2_cross(vector<int> & LSC, vector<int> tr2) {
	/*cout << "Before tr2 \n";
	for (int i = 0; i < tr2.size(); i++) {
	cout << tr2[i] << " ";
	}
	cout << "\n";
	print_cross(LSC);*/
	if (tr2[0] == 1) {
		swap(LSC[0], LSC[7]);
		swap(LSC[N + 0], LSC[N + 7]);
	}
	//print_LSM(LSM);
	if (tr2[1] == 1) {
		swap(LSC[1], LSC[6]);
		swap(LSC[N + 1], LSC[N + 6]);
	}
	//print_LSM(LSM);
	if (tr2[2] == 1) {
		swap(LSC[2], LSC[5]);
		swap(LSC[N + 2], LSC[N + 5]);
	}
	//print_LSM(LSM);
	if (tr2[3] == 1) {
		swap(LSC[3], LSC[4]);
		swap(LSC[N + 3], LSC[N + 4]);
	}
	//cout << "After tr2\n";
	//print_cross(LSC);

}

static inline void Transform4_cross(vector<int> & LSC, vector<int> tr4) {
	/*cout << "before tr4\n";
	for (int i = 0; i < tr4.size(); i++) {
	cout << tr4[i] << " ";
	}
	cout << "\n";
	print_cross(LSC);*/
	vector<int> t(LSC);
	for (int i = 0; i < N / 2; i++) {
		t[i] = LSC[tr4[i] - 1];
		t[N - i - 1] = LSC[N - 1 - tr4[i] + 1];

		t[N + i] = LSC[N + tr4[i] - 1];
		t[N + N - i - 1] = LSC[2 * N - 1 - tr4[i] + 1];
	}
	LSC = t;
	//cout << "After tr4\n";
	//print_cross(LSC);
}


int PROCESS_LSC(vector<int> LSC) {
	//vector<vector<int>> ST;
	set<vector<int>> ST;
	bool r = true;
	for (int i = 0; i < TR2.size(); i++) {
		for (int j = 0; j < TR4_cross.size(); j++) {
			vector<int> t(LSC);
			Transform2_cross(t, TR2[i]);
			Transform4_cross(t, TR4_cross[j]);

			vector<int> th(t);
			flip_horizontal_cross(th);

			vector<int> tv(t);
			flip_vertical_cross(tv);

			//vector<int> thv(th);
			//flip_vertical(thv);

			vector<int> ttt(t);
			Transpond(ttt);

			vector<int> tttv(ttt);
			flip_vertical_cross(tttv);

			normalize(ttt);
			normalize(tttv);


			normalize(t);
			normalize(th);
			normalize(tv);
			//normalize(thv);				

			ST.insert(t);
			ST.insert(th);
			ST.insert(tv);
			ST.insert(ttt);
			ST.insert(tttv);
			//ST.insert(thv);
			if (*ST.begin() != LSC) {
				r = false;
				return 0;
			}
		}
	}
	if (r == true) {
		return ST.size();
	}
}

void Reset()
{
	for (int i = 0; i < N*N; i++)
		LS[i] = 0;
	for (int i = 0; i < N; i++)
		Columns[i] = 0;
	for (int i = 0; i < N; i++)
		Rows[i] = 0;
	MD = 0;
	AD = 0;
	for (int j = 0; j<N; j++) {
		LS[j*N + j] = 1 << j;
		Rows[j] |= LS[j*N + j];
		Columns[j] |= LS[j*N + j];
		MD |= LS[j*N + j];
		if (j == N - 1 - j) {
			AD |= LS[j*N + j];
		}
	}
}

void Generate_crosses()
{
	uint64_t count_diag = 0;
	for (CR[56] = AllN ^ (Rows[7] | Columns[0] | AD); CR[56] != 0; CR[56] = (CR[56] - 1) & CR[56]) {
		LS[56] = (-CR[56]) & CR[56];
		Rows[7] |= LS[56];
		Columns[0] |= LS[56];
		AD |= LS[56];
		for (CR[49] = AllN ^ (Rows[6] | Columns[1] | AD); CR[49] != 0; CR[49] = (CR[49] - 1) & CR[49]) {
			LS[49] = (-CR[49]) & CR[49];
			Rows[6] |= LS[49];
			Columns[1] |= LS[49];
			AD |= LS[49];
			for (CR[42] = AllN ^ (Rows[5] | Columns[2] | AD); CR[42] != 0; CR[42] = (CR[42] - 1) & CR[42]) {
				LS[42] = (-CR[42]) & CR[42];
				Rows[5] |= LS[42];
				Columns[2] |= LS[42];
				AD |= LS[42];
				for (CR[35] = AllN ^ (Rows[4] | Columns[3] | AD); CR[35] != 0; CR[35] = (CR[35] - 1) & CR[35]) {
					LS[35] = (-CR[35]) & CR[35];
					Rows[4] |= LS[35];
					Columns[3] |= LS[35];
					AD |= LS[35];
					for (CR[28] = AllN ^ (Rows[3] | Columns[4] | AD); CR[28] != 0; CR[28] = (CR[28] - 1) & CR[28]) {
						LS[28] = (-CR[28]) & CR[28];
						Rows[3] |= LS[28];
						Columns[4] |= LS[28];
						AD |= LS[28];
						for (CR[21] = AllN ^ (Rows[2] | Columns[5] | AD); CR[21] != 0; CR[21] = (CR[21] - 1) & CR[21]) {
							LS[21] = (-CR[21]) & CR[21];
							Rows[2] |= LS[21];
							Columns[5] |= LS[21];
							AD |= LS[21];
							for (CR[14] = AllN ^ (Rows[1] | Columns[6] | AD); CR[14] != 0; CR[14] = (CR[14] - 1) & CR[14]) {
								LS[14] = (-CR[14]) & CR[14];
								Rows[1] |= LS[14];
								Columns[6] |= LS[14];
								AD |= LS[14];
								CR[7] = AllN ^ (Rows[0] | Columns[7] | AD);
								if (CR[7] != 0) {
									LS[7] = (-CR[7]) & CR[7];
									Rows[0] |= LS[7];
									Columns[7] |= LS[7];
									for (CR[1] = AllN ^ (Rows[0] | Columns[1]); CR[1] != 0; CR[1] = (CR[1] - 1) & CR[1]) {

										LS[1] = (-CR[1]) & CR[1];
										Rows[0] |= LS[1];
										Columns[1] |= LS[1];

										vector<int> cross = {
											LS[0],  LS[9], LS[18], LS[27], LS[36], LS[45], LS[54], LS[63],
											LS[7], LS[14], LS[21], LS[28], LS[35], LS[42], LS[49], LS[56]
										};

										int m = PROCESS_LSC(cross);
										if (m>0) {
											count_diag++;
											cout << count_diag << "\n";
										}

										Rows[0] ^= LS[1];
										Columns[1] ^= LS[1];

									}
									Rows[0] ^= LS[7];
									Columns[7] ^= LS[7];
								}
								Rows[1] ^= LS[14];
								Columns[6] ^= LS[14];
								AD ^= LS[14];
							}
							Rows[2] ^= LS[21];
							Columns[5] ^= LS[21];
							AD ^= LS[21];
						}
						Rows[3] ^= LS[28];
						Columns[4] ^= LS[28];
						AD ^= LS[28];
					}
					Rows[4] ^= LS[35];
					Columns[3] ^= LS[35];
					AD ^= LS[35];
				}
				Rows[5] ^= LS[42];
				Columns[2] ^= LS[42];
				AD ^= LS[42];
			}
			Rows[6] ^= LS[49];
			Columns[1] ^= LS[49];
			AD ^= LS[49];
		}
		Rows[7] ^= LS[56];
		Columns[0] ^= LS[56];
		AD ^= LS[56];
	}
	cout << "Number of crosses: " << count_diag << "\n";
}



void Generate();

void main()
{
	Reset();
	for (int i = 0; i < N; i++) { trf3[1 << i] = i; }

	Generate_crosses();
	getchar();
	double t1 = cpuTime();
	Reset();
	Generate();

	double t2 = cpuTime();
	cout << "Total squares count: " << SquaresCnt << endl;
	cout << "Time spent: " << t2 - t1 << endl;
	cout << "OK";
	getchar();
}
void Generate()
{
	double t_start = cpuTime();
	uint64_t TotalCnt = 0;
	int multiple = 0;
	for (CR[56] = AllN ^ (Rows[7] | Columns[0] | AD); CR[56] != 0; CR[56] = (CR[56] - 1) & CR[56]) {
		LS[56] = (-CR[56]) & CR[56];
		Rows[7] |= LS[56];
		Columns[0] |= LS[56];
		AD |= LS[56];
		for (CR[49] = AllN ^ (Rows[6] | Columns[1] | AD); CR[49] != 0; CR[49] = (CR[49] - 1) & CR[49]) {
			LS[49] = (-CR[49]) & CR[49];
			Rows[6] |= LS[49];
			Columns[1] |= LS[49];
			AD |= LS[49];
			for (CR[42] = AllN ^ (Rows[5] | Columns[2] | AD); CR[42] != 0; CR[42] = (CR[42] - 1) & CR[42]) {
				LS[42] = (-CR[42]) & CR[42];
				Rows[5] |= LS[42];
				Columns[2] |= LS[42];
				AD |= LS[42];
				for (CR[35] = AllN ^ (Rows[4] | Columns[3] | AD); CR[35] != 0; CR[35] = (CR[35] - 1) & CR[35]) {
					LS[35] = (-CR[35]) & CR[35];
					Rows[4] |= LS[35];
					Columns[3] |= LS[35];
					AD |= LS[35];
					for (CR[28] = AllN ^ (Rows[3] | Columns[4] | AD); CR[28] != 0; CR[28] = (CR[28] - 1) & CR[28]) {
						LS[28] = (-CR[28]) & CR[28];
						Rows[3] |= LS[28];
						Columns[4] |= LS[28];
						AD |= LS[28];
						for (CR[21] = AllN ^ (Rows[2] | Columns[5] | AD); CR[21] != 0; CR[21] = (CR[21] - 1) & CR[21]) {
							LS[21] = (-CR[21]) & CR[21];
							Rows[2] |= LS[21];
							Columns[5] |= LS[21];
							AD |= LS[21];
							for (CR[14] = AllN ^ (Rows[1] | Columns[6] | AD); CR[14] != 0; CR[14] = (CR[14] - 1) & CR[14]) {
								LS[14] = (-CR[14]) & CR[14];
								Rows[1] |= LS[14];
								Columns[6] |= LS[14];
								AD |= LS[14];
								CR[7] = AllN ^ (Rows[0] | Columns[7] | AD);
								if (CR[7] != 0) {
									LS[7] = (-CR[7]) & CR[7];
									Rows[0] |= LS[7];
									Columns[7] |= LS[7];
									for (CR[1] = AllN ^ (Rows[0] | Columns[1]); CR[1] != 0; CR[1] = (CR[1] - 1) & CR[1]) {

										LS[1] = (-CR[1]) & CR[1];
										Rows[0] |= LS[1];
										Columns[1] |= LS[1];

										vector<int> cross = {
											LS[0],  LS[9], LS[18], LS[27], LS[36], LS[45], LS[54], LS[63],
											LS[7], LS[14], LS[21], LS[28], LS[35], LS[42], LS[49], LS[56]
										};
										TotalCnt += SquaresCnt*multiple;

										multiple = PROCESS_LSC(cross);
										SquaresCnt = 0;
										if (multiple != 0) {
											cout << "Total count " << TotalCnt << endl;
											double t_cur = cpuTime();
											cout << "Average performance " << (double)TotalCnt / (t_cur - t_start) << endl;

											for (CR[2] = AllN ^ (Rows[0] | Columns[2]); CR[2] != 0; CR[2] = (CR[2] - 1) & CR[2]) {
												LS[2] = (-CR[2]) & CR[2];
												Rows[0] |= LS[2];
												Columns[2] |= LS[2];
												for (CR[3] = AllN ^ (Rows[0] | Columns[3]); CR[3] != 0; CR[3] = (CR[3] - 1) & CR[3]) {
													LS[3] = (-CR[3]) & CR[3];
													Rows[0] |= LS[3];
													Columns[3] |= LS[3];
													for (CR[4] = AllN ^ (Rows[0] | Columns[4]); CR[4] != 0; CR[4] = (CR[4] - 1) & CR[4]) {
														LS[4] = (-CR[4]) & CR[4];
														Rows[0] |= LS[4];
														Columns[4] |= LS[4];
														for (CR[5] = AllN ^ (Rows[0] | Columns[5]); CR[5] != 0; CR[5] = (CR[5] - 1) & CR[5]) {
															LS[5] = (-CR[5]) & CR[5];
															Rows[0] |= LS[5];
															Columns[5] |= LS[5];
															CR[6] = AllN ^ (Rows[0] | Columns[6]);
															if (CR[6] != 0) {
																LS[6] = (-CR[6]) & CR[6];
																Columns[6] |= LS[6];
																for (CR[10] = AllN ^ (Rows[1] | Columns[2]); CR[10] != 0; CR[10] = (CR[10] - 1) & CR[10]) {
																	LS[10] = (-CR[10]) & CR[10];
																	Rows[1] |= LS[10];
																	Columns[2] |= LS[10];
																	for (CR[11] = AllN ^ (Rows[1] | Columns[3]); CR[11] != 0; CR[11] = (CR[11] - 1) & CR[11]) {
																		LS[11] = (-CR[11]) & CR[11];
																		Rows[1] |= LS[11];
																		Columns[3] |= LS[11];
																		for (CR[12] = AllN ^ (Rows[1] | Columns[4]); CR[12] != 0; CR[12] = (CR[12] - 1) & CR[12]) {
																			LS[12] = (-CR[12]) & CR[12];
																			Rows[1] |= LS[12];
																			Columns[4] |= LS[12];
																			for (CR[13] = AllN ^ (Rows[1] | Columns[5]); CR[13] != 0; CR[13] = (CR[13] - 1) & CR[13]) {
																				LS[13] = (-CR[13]) & CR[13];
																				Rows[1] |= LS[13];
																				Columns[5] |= LS[13];
																				for (CR[8] = AllN ^ (Rows[1] | Columns[0]); CR[8] != 0; CR[8] = (CR[8] - 1) & CR[8]) {
																					LS[8] = (-CR[8]) & CR[8];
																					Rows[1] |= LS[8];
																					Columns[0] |= LS[8];
																					CR[15] = AllN ^ (Rows[1] | Columns[7]);
																					if (CR[15] != 0) {
																						LS[15] = (-CR[15]) & CR[15];
																						Columns[7] |= LS[15];
																						for (CR[19] = AllN ^ (Rows[2] | Columns[3]); CR[19] != 0; CR[19] = (CR[19] - 1) & CR[19]) {
																							LS[19] = (-CR[19]) & CR[19];
																							Rows[2] |= LS[19];
																							Columns[3] |= LS[19];
																							for (CR[20] = AllN ^ (Rows[2] | Columns[4]); CR[20] != 0; CR[20] = (CR[20] - 1) & CR[20]) {
																								LS[20] = (-CR[20]) & CR[20];
																								Rows[2] |= LS[20];
																								Columns[4] |= LS[20];
																								for (CR[16] = AllN ^ (Rows[2] | Columns[0]); CR[16] != 0; CR[16] = (CR[16] - 1) & CR[16]) {
																									LS[16] = (-CR[16]) & CR[16];
																									Rows[2] |= LS[16];
																									Columns[0] |= LS[16];
																									for (CR[17] = AllN ^ (Rows[2] | Columns[1]); CR[17] != 0; CR[17] = (CR[17] - 1) & CR[17]) {
																										LS[17] = (-CR[17]) & CR[17];
																										Rows[2] |= LS[17];
																										Columns[1] |= LS[17];
																										for (CR[22] = AllN ^ (Rows[2] | Columns[6]); CR[22] != 0; CR[22] = (CR[22] - 1) & CR[22]) {
																											LS[22] = (-CR[22]) & CR[22];
																											Rows[2] |= LS[22];
																											Columns[6] |= LS[22];
																											CR[23] = AllN ^ (Rows[2] | Columns[7]);
																											if (CR[23] != 0) {
																												LS[23] = (-CR[23]) & CR[23];
																												Columns[7] |= LS[23];
																												for (CR[43] = AllN ^ (Rows[5] | Columns[3]); CR[43] != 0; CR[43] = (CR[43] - 1) & CR[43]) {
																													LS[43] = (-CR[43]) & CR[43];
																													Rows[5] |= LS[43];
																													Columns[3] |= LS[43];
																													for (CR[44] = AllN ^ (Rows[5] | Columns[4]); CR[44] != 0; CR[44] = (CR[44] - 1) & CR[44]) {
																														LS[44] = (-CR[44]) & CR[44];
																														Rows[5] |= LS[44];
																														Columns[4] |= LS[44];
																														for (CR[40] = AllN ^ (Rows[5] | Columns[0]); CR[40] != 0; CR[40] = (CR[40] - 1) & CR[40]) {
																															LS[40] = (-CR[40]) & CR[40];
																															Rows[5] |= LS[40];
																															Columns[0] |= LS[40];
																															for (CR[41] = AllN ^ (Rows[5] | Columns[1]); CR[41] != 0; CR[41] = (CR[41] - 1) & CR[41]) {
																																LS[41] = (-CR[41]) & CR[41];
																																Rows[5] |= LS[41];
																																Columns[1] |= LS[41];
																																for (CR[46] = AllN ^ (Rows[5] | Columns[6]); CR[46] != 0; CR[46] = (CR[46] - 1) & CR[46]) {
																																	LS[46] = (-CR[46]) & CR[46];
																																	Rows[5] |= LS[46];
																																	Columns[6] |= LS[46];
																																	CR[47] = AllN ^ (Rows[5] | Columns[7]);
																																	if (CR[47] != 0) {
																																		LS[47] = (-CR[47]) & CR[47];
																																		Columns[7] |= LS[47];
																																		for (CR[51] = AllN ^ (Rows[6] | Columns[3]); CR[51] != 0; CR[51] = (CR[51] - 1) & CR[51]) {
																																			LS[51] = (-CR[51]) & CR[51];
																																			Rows[6] |= LS[51];
																																			Columns[3] |= LS[51];
																																			CR[59] = AllN ^ (Rows[7] | Columns[3]);
																																			if (CR[59] != 0) {
																																				LS[59] = (-CR[59]) & CR[59];
																																				Rows[7] |= LS[59];
																																				for (CR[52] = AllN ^ (Rows[6] | Columns[4]); CR[52] != 0; CR[52] = (CR[52] - 1) & CR[52]) {
																																					LS[52] = (-CR[52]) & CR[52];
																																					Rows[6] |= LS[52];
																																					Columns[4] |= LS[52];
																																					CR[60] = AllN ^ (Rows[7] | Columns[4]);
																																					if (CR[60] != 0) {
																																						LS[60] = (-CR[60]) & CR[60];
																																						Rows[7] |= LS[60];
																																						for (CR[48] = AllN ^ (Rows[6] | Columns[0]); CR[48] != 0; CR[48] = (CR[48] - 1) & CR[48]) {
																																							LS[48] = (-CR[48]) & CR[48];
																																							Rows[6] |= LS[48];
																																							Columns[0] |= LS[48];
																																							for (CR[55] = AllN ^ (Rows[6] | Columns[7]); CR[55] != 0; CR[55] = (CR[55] - 1) & CR[55]) {
																																								LS[55] = (-CR[55]) & CR[55];
																																								Rows[6] |= LS[55];
																																								Columns[7] |= LS[55];
																																								for (CR[50] = AllN ^ (Rows[6] | Columns[2]); CR[50] != 0; CR[50] = (CR[50] - 1) & CR[50]) {
																																									LS[50] = (-CR[50]) & CR[50];
																																									Rows[6] |= LS[50];
																																									Columns[2] |= LS[50];
																																									CR[53] = AllN ^ (Rows[6] | Columns[5]);
																																									if (CR[53] != 0) {
																																										LS[53] = (-CR[53]) & CR[53];
																																										Columns[5] |= LS[53];
																																										for (CR[57] = AllN ^ (Rows[7] | Columns[1]); CR[57] != 0; CR[57] = (CR[57] - 1) & CR[57]) {
																																											LS[57] = (-CR[57]) & CR[57];
																																											Rows[7] |= LS[57];
																																											Columns[1] |= LS[57];
																																											for (CR[58] = AllN ^ (Rows[7] | Columns[2]); CR[58] != 0; CR[58] = (CR[58] - 1) & CR[58]) {
																																												LS[58] = (-CR[58]) & CR[58];
																																												Rows[7] |= LS[58];
																																												Columns[2] |= LS[58];
																																												for (CR[61] = AllN ^ (Rows[7] | Columns[5]); CR[61] != 0; CR[61] = (CR[61] - 1) & CR[61]) {
																																													LS[61] = (-CR[61]) & CR[61];
																																													Rows[7] |= LS[61];
																																													Columns[5] |= LS[61];
																																													CR[62] = AllN ^ (Rows[7] | Columns[6]);
																																													if (CR[62] != 0) {
																																														LS[62] = (-CR[62]) & CR[62];
																																														Columns[6] |= LS[62];
																																														for (CR[24] = AllN ^ (Rows[3] | Columns[0]); CR[24] != 0; CR[24] = (CR[24] - 1) & CR[24]) {
																																															LS[24] = (-CR[24]) & CR[24];
																																															Rows[3] |= LS[24];
																																															Columns[0] |= LS[24];
																																															CR[32] = AllN ^ (Rows[4] | Columns[0]);
																																															if (CR[32] != 0) {
																																																LS[32] = (-CR[32]) & CR[32];
																																																Rows[4] |= LS[32];
																																																for (CR[25] = AllN ^ (Rows[3] | Columns[1]); CR[25] != 0; CR[25] = (CR[25] - 1) & CR[25]) {
																																																	LS[25] = (-CR[25]) & CR[25];
																																																	Rows[3] |= LS[25];
																																																	Columns[1] |= LS[25];
																																																	CR[33] = AllN ^ (Rows[4] | Columns[1]);
																																																	if (CR[33] != 0) {
																																																		LS[33] = (-CR[33]) & CR[33];
																																																		Rows[4] |= LS[33];
																																																		for (CR[26] = AllN ^ (Rows[3] | Columns[2]); CR[26] != 0; CR[26] = (CR[26] - 1) & CR[26]) {
																																																			LS[26] = (-CR[26]) & CR[26];
																																																			Rows[3] |= LS[26];
																																																			Columns[2] |= LS[26];
																																																			CR[34] = AllN ^ (Rows[4] | Columns[2]);
																																																			if (CR[34] != 0) {
																																																				LS[34] = (-CR[34]) & CR[34];
																																																				Rows[4] |= LS[34];
																																																				for (CR[29] = AllN ^ (Rows[3] | Columns[5]); CR[29] != 0; CR[29] = (CR[29] - 1) & CR[29]) {
																																																					LS[29] = (-CR[29]) & CR[29];
																																																					Rows[3] |= LS[29];
																																																					Columns[5] |= LS[29];
																																																					CR[37] = AllN ^ (Rows[4] | Columns[5]);
																																																					if (CR[37] != 0) {
																																																						LS[37] = (-CR[37]) & CR[37];
																																																						Rows[4] |= LS[37];
																																																						for (CR[30] = AllN ^ (Rows[3] | Columns[6]); CR[30] != 0; CR[30] = (CR[30] - 1) & CR[30]) {
																																																							LS[30] = (-CR[30]) & CR[30];
																																																							Rows[3] |= LS[30];
																																																							Columns[6] |= LS[30];
																																																							CR[31] = AllN ^ (Rows[3] | Columns[7]);
																																																							if (CR[31] != 0) {
																																																								LS[31] = (-CR[31]) & CR[31];
																																																								Columns[7] |= LS[31];
																																																								CR[38] = AllN ^ (Rows[4] | Columns[6]);
																																																								if (CR[38] != 0) {
																																																									LS[38] = (-CR[38]) & CR[38];
																																																									Rows[4] |= LS[38];
																																																									CR[39] = AllN ^ (Rows[4] | Columns[7]);
																																																									if (CR[39] != 0) {
																																																										LS[39] = (-CR[39]) & CR[39];
																																																										SquaresCnt++;
																																																										if (SquaresCnt % 10000000 == 0) {
																																																											cout << "Squares found: " << SquaresCnt << endl;
																																																										}
																																																									}
																																																									Rows[4] ^= LS[38];
																																																								}
																																																								Columns[7] ^= LS[31];
																																																							}
																																																							Rows[3] ^= LS[30];
																																																							Columns[6] ^= LS[30];
																																																						}
																																																						Rows[4] ^= LS[37];
																																																					}
																																																					Rows[3] ^= LS[29];
																																																					Columns[5] ^= LS[29];
																																																				}
																																																				Rows[4] ^= LS[34];
																																																			}
																																																			Rows[3] ^= LS[26];
																																																			Columns[2] ^= LS[26];
																																																		}
																																																		Rows[4] ^= LS[33];
																																																	}
																																																	Rows[3] ^= LS[25];
																																																	Columns[1] ^= LS[25];
																																																}
																																																Rows[4] ^= LS[32];
																																															}
																																															Rows[3] ^= LS[24];
																																															Columns[0] ^= LS[24];
																																														}
																																														Columns[6] ^= LS[62];
																																													}
																																													Rows[7] ^= LS[61];
																																													Columns[5] ^= LS[61];
																																												}
																																												Rows[7] ^= LS[58];
																																												Columns[2] ^= LS[58];
																																											}
																																											Rows[7] ^= LS[57];
																																											Columns[1] ^= LS[57];
																																										}
																																										Columns[5] ^= LS[53];
																																									}
																																									Rows[6] ^= LS[50];
																																									Columns[2] ^= LS[50];
																																								}
																																								Rows[6] ^= LS[55];
																																								Columns[7] ^= LS[55];
																																							}
																																							Rows[6] ^= LS[48];
																																							Columns[0] ^= LS[48];
																																						}
																																						Rows[7] ^= LS[60];
																																					}
																																					Rows[6] ^= LS[52];
																																					Columns[4] ^= LS[52];
																																				}
																																				Rows[7] ^= LS[59];
																																			}
																																			Rows[6] ^= LS[51];
																																			Columns[3] ^= LS[51];
																																		}
																																		Columns[7] ^= LS[47];
																																	}
																																	Rows[5] ^= LS[46];
																																	Columns[6] ^= LS[46];
																																}
																																Rows[5] ^= LS[41];
																																Columns[1] ^= LS[41];
																															}
																															Rows[5] ^= LS[40];
																															Columns[0] ^= LS[40];
																														}
																														Rows[5] ^= LS[44];
																														Columns[4] ^= LS[44];
																													}
																													Rows[5] ^= LS[43];
																													Columns[3] ^= LS[43];
																												}
																												Columns[7] ^= LS[23];
																											}
																											Rows[2] ^= LS[22];
																											Columns[6] ^= LS[22];
																										}
																										Rows[2] ^= LS[17];
																										Columns[1] ^= LS[17];
																									}
																									Rows[2] ^= LS[16];
																									Columns[0] ^= LS[16];
																								}
																								Rows[2] ^= LS[20];
																								Columns[4] ^= LS[20];
																							}
																							Rows[2] ^= LS[19];
																							Columns[3] ^= LS[19];
																						}
																						Columns[7] ^= LS[15];
																					}
																					Rows[1] ^= LS[8];
																					Columns[0] ^= LS[8];
																				}
																				Rows[1] ^= LS[13];
																				Columns[5] ^= LS[13];
																			}
																			Rows[1] ^= LS[12];
																			Columns[4] ^= LS[12];
																		}
																		Rows[1] ^= LS[11];
																		Columns[3] ^= LS[11];
																	}
																	Rows[1] ^= LS[10];
																	Columns[2] ^= LS[10];
																}
																Columns[6] ^= LS[6];
															}
															Rows[0] ^= LS[5];
															Columns[5] ^= LS[5];
														}
														Rows[0] ^= LS[4];
														Columns[4] ^= LS[4];
													}
													Rows[0] ^= LS[3];
													Columns[3] ^= LS[3];
												}
												Rows[0] ^= LS[2];
												Columns[2] ^= LS[2];
											}

										}
										Rows[0] ^= LS[1];
										Columns[1] ^= LS[1];

									}
									Rows[0] ^= LS[7];
									Columns[7] ^= LS[7];
								}
								Rows[1] ^= LS[14];
								Columns[6] ^= LS[14];
								AD ^= LS[14];
							}
							Rows[2] ^= LS[21];
							Columns[5] ^= LS[21];
							AD ^= LS[21];
						}
						Rows[3] ^= LS[28];
						Columns[4] ^= LS[28];
						AD ^= LS[28];
					}
					Rows[4] ^= LS[35];
					Columns[3] ^= LS[35];
					AD ^= LS[35];
				}
				Rows[5] ^= LS[42];
				Columns[2] ^= LS[42];
				AD ^= LS[42];
			}
			Rows[6] ^= LS[49];
			Columns[1] ^= LS[49];
			AD ^= LS[49];
		}
		Rows[7] ^= LS[56];
		Columns[0] ^= LS[56];
		AD ^= LS[56];
	}
	cout << "Total count: " << TotalCnt << "\n";
}
