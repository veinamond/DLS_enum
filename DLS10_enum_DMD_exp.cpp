#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <set>
static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
using namespace std;
const int N = 10;
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
	{ 0,0,0,0,0 },{ 0,1,0,0,0 },{ 0,0,1,0,0 },{ 0,1,1,0,0 },
	{ 0,0,0,1,0 },{ 0,1,0,1,0 },{ 0,0,1,1,0 },{ 0,1,1,1,0 },
	{ 0,0,0,0,1 },{ 0, 1,0,0,1 },{ 0,0,1,0,1 },{ 0,1,1,0,1 },
	{ 0,0,0,1,1 },{ 0, 1,0,1,1 },{ 0,0,1,1,1 },{ 0,1,1,1,1 },
	{1,0,0,0,0 },{ 1, 1,0,0,0 },{ 1,0,1,0,0 },{ 1,1,1,0,0 },
	{ 1,0,0,1,0 },{ 1,1,0,1,0 },{ 1,0,1,1,0 },{ 1,1,1,1,0 },
	{ 1,0,0,0,1 },{ 1, 1,0,0,1 },{ 1,0,1,0,1 },{ 1, 1,1,0,1 },
	{ 1,0,0,1,1 },{ 1,1,0,1,1 },{ 1, 0,1,1,1 },{ 1,1,1,1,1 }
};

vector<vector<int>> TR4 = {
	{ 1,2,3 },{ 1,3,2 },{ 2,1,3 },{ 2,3,1 },{ 3,1,2 },{ 3,2,1 }
};
vector<vector<int>> TR4_cross;

vector<vector<int>> gen_permutations(vector<int> src) {
	vector<int> t = src;
	vector<vector<int>> res;
	do {
		res.push_back(t);
	} while (next_permutation(t.begin(), t.end()));
	return res;
}
void print_cross(vector<int> LSC) {
	cout << trf3[LSC[0]] << "               " << trf3[LSC[N + 0]] << "\n";
	cout << "  " << trf3[LSC[1]] << "           " << trf3[LSC[N + 1]] << " " << "\n";
	cout << "    " << trf3[LSC[2]] << "       " << trf3[LSC[N + 2]] << "  " << "\n";
	cout << "     " << trf3[LSC[3]] << "     " << trf3[LSC[N + 3]] << "   " << "\n";
	cout << "       " << trf3[LSC[N + 4]] << " " << trf3[LSC[4]] << "   " << "\n";
	cout << "       " << trf3[LSC[N + 5]] << " " << trf3[LSC[5]] << "  " << "\n";
	cout << "     " << trf3[LSC[N + 6]] << "     " << trf3[LSC[6]] << " " << "\n";
	cout << "    "<<trf3[LSC[N + 7]] << "       "  << trf3[LSC[7]] << "\n";
	cout << "  "<< trf3[LSC[N + 8]] << "           " << trf3[LSC[8]] << "\n";
	cout << trf3[LSC[N + 9]] << "               " << trf3[LSC[9]] << "\n";
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
		swap(LSC[0], LSC[9]);
		swap(LSC[N + 0], LSC[N + 9]);
	}
	//print_LSM(LSM);
	if (tr2[1] == 1) {
		swap(LSC[1], LSC[8]);
		swap(LSC[N + 1], LSC[N + 8]);
	}
	//print_LSM(LSM);
	if (tr2[2] == 1) {
		swap(LSC[2], LSC[7]);
		swap(LSC[N + 2], LSC[N + 7]);
	}
	//print_LSM(LSM);
	if (tr2[3] == 1) {
		swap(LSC[3], LSC[6]);
		swap(LSC[N + 3], LSC[N + 6]);
	}
	if (tr2[4] == 1) {
		swap(LSC[4], LSC[5]);
		swap(LSC[N + 4], LSC[N + 5]);
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


static inline void Transpond(vector<int> & LSC) {
	/*cout << "before tr4\n";
	for (int i = 0; i < tr4.size(); i++) {
	cout << tr4[i] << " ";
	}
	cout << "\n";
	print_cross(LSC);*/
	swap(LSC[N], LSC[N + 9]);
	swap(LSC[N+1], LSC[N + 8]);
	swap(LSC[N + 2], LSC[N + 7]);
	swap(LSC[N + 3], LSC[N + 6]);
	swap(LSC[N + 4], LSC[N + 5]);
	
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
	uint64_t p = 0;
	for (CR[90] = AllN ^ (Rows[9] | Columns[0] | AD); CR[90] != 0; CR[90] = (CR[90] - 1) & CR[90]) {
		LS[90] = (-CR[90]) & CR[90];
		Rows[9] |= LS[90];
		Columns[0] |= LS[90];
		AD |= LS[90];
		for (CR[81] = AllN ^ (Rows[8] | Columns[1] | AD); CR[81] != 0; CR[81] = (CR[81] - 1) & CR[81]) {
			LS[81] = (-CR[81]) & CR[81];
			Rows[8] |= LS[81];
			Columns[1] |= LS[81];
			AD |= LS[81];
			for (CR[72] = AllN ^ (Rows[7] | Columns[2] | AD); CR[72] != 0; CR[72] = (CR[72] - 1) & CR[72]) {
				LS[72] = (-CR[72]) & CR[72];
				Rows[7] |= LS[72];
				Columns[2] |= LS[72];
				AD |= LS[72];
				for (CR[63] = AllN ^ (Rows[6] | Columns[3] | AD); CR[63] != 0; CR[63] = (CR[63] - 1) & CR[63]) {
					LS[63] = (-CR[63]) & CR[63];
					Rows[6] |= LS[63];
					Columns[3] |= LS[63];
					AD |= LS[63];
					for (CR[54] = AllN ^ (Rows[5] | Columns[4] | AD); CR[54] != 0; CR[54] = (CR[54] - 1) & CR[54]) {
						LS[54] = (-CR[54]) & CR[54];
						Rows[5] |= LS[54];
						Columns[4] |= LS[54];
						AD |= LS[54];
						for (CR[45] = AllN ^ (Rows[4] | Columns[5] | AD); CR[45] != 0; CR[45] = (CR[45] - 1) & CR[45]) {
							LS[45] = (-CR[45]) & CR[45];
							Rows[4] |= LS[45];
							Columns[5] |= LS[45];
							AD |= LS[45];
							for (CR[36] = AllN ^ (Rows[3] | Columns[6] | AD); CR[36] != 0; CR[36] = (CR[36] - 1) & CR[36]) {
								LS[36] = (-CR[36]) & CR[36];
								Rows[3] |= LS[36];
								Columns[6] |= LS[36];
								AD |= LS[36];
								for (CR[27] = AllN ^ (Rows[2] | Columns[7] | AD); CR[27] != 0; CR[27] = (CR[27] - 1) & CR[27]) {
									LS[27] = (-CR[27]) & CR[27];
									Rows[2] |= LS[27];
									Columns[7] |= LS[27];
									AD |= LS[27];
									for (CR[18] = AllN ^ (Rows[1] | Columns[8] | AD); CR[18] != 0; CR[18] = (CR[18] - 1) & CR[18]) {
										LS[18] = (-CR[18]) & CR[18];
										Rows[1] |= LS[18];
										Columns[8] |= LS[18];
										AD |= LS[18];
										CR[9] = AllN ^ (Rows[0] | Columns[9] | AD);
										if (CR[9] != 0) {
											LS[9] = (-CR[9]) & CR[9];
											vector<int> cross = {
												LS[0], LS[11], LS[22], LS[33], LS[44], LS[55], LS[66], LS[77], LS[88], LS[99],
												LS[9], LS[18], LS[27], LS[36], LS[45], LS[54], LS[63], LS[72], LS[81], LS[90]
											};
											p++;
											int m = PROCESS_LSC(cross);
											if (m>0) {
												count_diag++;	
												cout << count_diag << "\n";
												print_cross(cross);
											}
										
										}
										Rows[1] ^= LS[18];
										Columns[8] ^= LS[18];
										AD ^= LS[18];
									}
									Rows[2] ^= LS[27];
									Columns[7] ^= LS[27];
									AD ^= LS[27];
								}
								Rows[3] ^= LS[36];
								Columns[6] ^= LS[36];
								AD ^= LS[36];
							}
							Rows[4] ^= LS[45];
							Columns[5] ^= LS[45];
							AD ^= LS[45];
						}
						Rows[5] ^= LS[54];
						Columns[4] ^= LS[54];
						AD ^= LS[54];
					}
					Rows[6] ^= LS[63];
					Columns[3] ^= LS[63];
					AD ^= LS[63];
				}
				Rows[7] ^= LS[72];
				Columns[2] ^= LS[72];
				AD ^= LS[72];
			}
			Rows[8] ^= LS[81];
			Columns[1] ^= LS[81];
			AD ^= LS[81];
		}
		Rows[9] ^= LS[90];
		Columns[0] ^= LS[90];
		AD ^= LS[90];
	}
	cout << "Number of crosses: " << count_diag << "\n";
	cout << "Number of potential crosses: " << p << "\n";
}



void Generate();

void main()
{
	for (int i = 0; i < N; i++) { trf3[1 << i] = i; }
	TR4_cross = gen_permutations({ 1,2,3,4,5 });
	Reset();
	

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
	for (CR[90] = AllN ^ (Rows[9] | Columns[0] | AD); CR[90] != 0; CR[90] = (CR[90] - 1) & CR[90]) {
		LS[90] = (-CR[90]) & CR[90];
		Rows[9] |= LS[90];
		Columns[0] |= LS[90];
		AD |= LS[90];
		for (CR[81] = AllN ^ (Rows[8] | Columns[1] | AD); CR[81] != 0; CR[81] = (CR[81] - 1) & CR[81]) {
			LS[81] = (-CR[81]) & CR[81];
			Rows[8] |= LS[81];
			Columns[1] |= LS[81];
			AD |= LS[81];
			for (CR[72] = AllN ^ (Rows[7] | Columns[2] | AD); CR[72] != 0; CR[72] = (CR[72] - 1) & CR[72]) {
				LS[72] = (-CR[72]) & CR[72];
				Rows[7] |= LS[72];
				Columns[2] |= LS[72];
				AD |= LS[72];
				for (CR[63] = AllN ^ (Rows[6] | Columns[3] | AD); CR[63] != 0; CR[63] = (CR[63] - 1) & CR[63]) {
					LS[63] = (-CR[63]) & CR[63];
					Rows[6] |= LS[63];
					Columns[3] |= LS[63];
					AD |= LS[63];
					for (CR[54] = AllN ^ (Rows[5] | Columns[4] | AD); CR[54] != 0; CR[54] = (CR[54] - 1) & CR[54]) {
						LS[54] = (-CR[54]) & CR[54];
						Rows[5] |= LS[54];
						Columns[4] |= LS[54];
						AD |= LS[54];
						for (CR[45] = AllN ^ (Rows[4] | Columns[5] | AD); CR[45] != 0; CR[45] = (CR[45] - 1) & CR[45]) {
							LS[45] = (-CR[45]) & CR[45];
							Rows[4] |= LS[45];
							Columns[5] |= LS[45];
							AD |= LS[45];
							for (CR[36] = AllN ^ (Rows[3] | Columns[6] | AD); CR[36] != 0; CR[36] = (CR[36] - 1) & CR[36]) {
								LS[36] = (-CR[36]) & CR[36];
								Rows[3] |= LS[36];
								Columns[6] |= LS[36];
								AD |= LS[36];
								for (CR[27] = AllN ^ (Rows[2] | Columns[7] | AD); CR[27] != 0; CR[27] = (CR[27] - 1) & CR[27]) {
									LS[27] = (-CR[27]) & CR[27];
									Rows[2] |= LS[27];
									Columns[7] |= LS[27];
									AD |= LS[27];
									for (CR[18] = AllN ^ (Rows[1] | Columns[8] | AD); CR[18] != 0; CR[18] = (CR[18] - 1) & CR[18]) {
										LS[18] = (-CR[18]) & CR[18];
										Rows[1] |= LS[18];
										Columns[8] |= LS[18];
										AD |= LS[18];
										CR[9] = AllN ^ (Rows[0] | Columns[9] | AD);
										if (CR[9] != 0) {
											LS[9] = (-CR[9]) & CR[9];
											Rows[0] |= LS[9];
											Columns[9] |= LS[9];
											for (CR[1] = AllN ^ (Rows[0] | Columns[1]); CR[1] != 0; CR[1] = (CR[1] - 1) & CR[1]) {
												LS[1] = (-CR[1]) & CR[1];
												Rows[0] |= LS[1];
												Columns[1] |= LS[1];
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
																for (CR[6] = AllN ^ (Rows[0] | Columns[6]); CR[6] != 0; CR[6] = (CR[6] - 1) & CR[6]) {
																	LS[6] = (-CR[6]) & CR[6];
																	Rows[0] |= LS[6];
																	Columns[6] |= LS[6];
																	for (CR[7] = AllN ^ (Rows[0] | Columns[7]); CR[7] != 0; CR[7] = (CR[7] - 1) & CR[7]) {
																		LS[7] = (-CR[7]) & CR[7];
																		Rows[0] |= LS[7];
																		Columns[7] |= LS[7];
																		CR[8] = AllN ^ (Rows[0] | Columns[8]);
																		if (CR[8] != 0) {
																			LS[8] = (-CR[8]) & CR[8];
																			Columns[8] |= LS[8];
																			for (CR[12] = AllN ^ (Rows[1] | Columns[2]); CR[12] != 0; CR[12] = (CR[12] - 1) & CR[12]) {
																				LS[12] = (-CR[12]) & CR[12];
																				Rows[1] |= LS[12];
																				Columns[2] |= LS[12];
																				for (CR[13] = AllN ^ (Rows[1] | Columns[3]); CR[13] != 0; CR[13] = (CR[13] - 1) & CR[13]) {
																					LS[13] = (-CR[13]) & CR[13];
																					Rows[1] |= LS[13];
																					Columns[3] |= LS[13];
																					for (CR[14] = AllN ^ (Rows[1] | Columns[4]); CR[14] != 0; CR[14] = (CR[14] - 1) & CR[14]) {
																						LS[14] = (-CR[14]) & CR[14];
																						Rows[1] |= LS[14];
																						Columns[4] |= LS[14];
																						for (CR[15] = AllN ^ (Rows[1] | Columns[5]); CR[15] != 0; CR[15] = (CR[15] - 1) & CR[15]) {
																							LS[15] = (-CR[15]) & CR[15];
																							Rows[1] |= LS[15];
																							Columns[5] |= LS[15];
																							for (CR[16] = AllN ^ (Rows[1] | Columns[6]); CR[16] != 0; CR[16] = (CR[16] - 1) & CR[16]) {
																								LS[16] = (-CR[16]) & CR[16];
																								Rows[1] |= LS[16];
																								Columns[6] |= LS[16];
																								for (CR[17] = AllN ^ (Rows[1] | Columns[7]); CR[17] != 0; CR[17] = (CR[17] - 1) & CR[17]) {
																									LS[17] = (-CR[17]) & CR[17];
																									Rows[1] |= LS[17];
																									Columns[7] |= LS[17];
																									for (CR[10] = AllN ^ (Rows[1] | Columns[0]); CR[10] != 0; CR[10] = (CR[10] - 1) & CR[10]) {
																										LS[10] = (-CR[10]) & CR[10];
																										Rows[1] |= LS[10];
																										Columns[0] |= LS[10];
																										CR[19] = AllN ^ (Rows[1] | Columns[9]);
																										if (CR[19] != 0) {
																											LS[19] = (-CR[19]) & CR[19];
																											Columns[9] |= LS[19];
																											for (CR[23] = AllN ^ (Rows[2] | Columns[3]); CR[23] != 0; CR[23] = (CR[23] - 1) & CR[23]) {
																												LS[23] = (-CR[23]) & CR[23];
																												Rows[2] |= LS[23];
																												Columns[3] |= LS[23];
																												for (CR[24] = AllN ^ (Rows[2] | Columns[4]); CR[24] != 0; CR[24] = (CR[24] - 1) & CR[24]) {
																													LS[24] = (-CR[24]) & CR[24];
																													Rows[2] |= LS[24];
																													Columns[4] |= LS[24];
																													for (CR[25] = AllN ^ (Rows[2] | Columns[5]); CR[25] != 0; CR[25] = (CR[25] - 1) & CR[25]) {
																														LS[25] = (-CR[25]) & CR[25];
																														Rows[2] |= LS[25];
																														Columns[5] |= LS[25];
																														for (CR[26] = AllN ^ (Rows[2] | Columns[6]); CR[26] != 0; CR[26] = (CR[26] - 1) & CR[26]) {
																															LS[26] = (-CR[26]) & CR[26];
																															Rows[2] |= LS[26];
																															Columns[6] |= LS[26];
																															for (CR[20] = AllN ^ (Rows[2] | Columns[0]); CR[20] != 0; CR[20] = (CR[20] - 1) & CR[20]) {
																																LS[20] = (-CR[20]) & CR[20];
																																Rows[2] |= LS[20];
																																Columns[0] |= LS[20];
																																for (CR[21] = AllN ^ (Rows[2] | Columns[1]); CR[21] != 0; CR[21] = (CR[21] - 1) & CR[21]) {
																																	LS[21] = (-CR[21]) & CR[21];
																																	Rows[2] |= LS[21];
																																	Columns[1] |= LS[21];
																																	for (CR[28] = AllN ^ (Rows[2] | Columns[8]); CR[28] != 0; CR[28] = (CR[28] - 1) & CR[28]) {
																																		LS[28] = (-CR[28]) & CR[28];
																																		Rows[2] |= LS[28];
																																		Columns[8] |= LS[28];
																																		CR[29] = AllN ^ (Rows[2] | Columns[9]);
																																		if (CR[29] != 0) {
																																			LS[29] = (-CR[29]) & CR[29];
																																			Columns[9] |= LS[29];
																																			for (CR[34] = AllN ^ (Rows[3] | Columns[4]); CR[34] != 0; CR[34] = (CR[34] - 1) & CR[34]) {
																																				LS[34] = (-CR[34]) & CR[34];
																																				Rows[3] |= LS[34];
																																				Columns[4] |= LS[34];
																																				for (CR[35] = AllN ^ (Rows[3] | Columns[5]); CR[35] != 0; CR[35] = (CR[35] - 1) & CR[35]) {
																																					LS[35] = (-CR[35]) & CR[35];
																																					Rows[3] |= LS[35];
																																					Columns[5] |= LS[35];
																																					for (CR[30] = AllN ^ (Rows[3] | Columns[0]); CR[30] != 0; CR[30] = (CR[30] - 1) & CR[30]) {
																																						LS[30] = (-CR[30]) & CR[30];
																																						Rows[3] |= LS[30];
																																						Columns[0] |= LS[30];
																																						for (CR[31] = AllN ^ (Rows[3] | Columns[1]); CR[31] != 0; CR[31] = (CR[31] - 1) & CR[31]) {
																																							LS[31] = (-CR[31]) & CR[31];
																																							Rows[3] |= LS[31];
																																							Columns[1] |= LS[31];
																																							for (CR[32] = AllN ^ (Rows[3] | Columns[2]); CR[32] != 0; CR[32] = (CR[32] - 1) & CR[32]) {
																																								LS[32] = (-CR[32]) & CR[32];
																																								Rows[3] |= LS[32];
																																								Columns[2] |= LS[32];
																																								for (CR[37] = AllN ^ (Rows[3] | Columns[7]); CR[37] != 0; CR[37] = (CR[37] - 1) & CR[37]) {
																																									LS[37] = (-CR[37]) & CR[37];
																																									Rows[3] |= LS[37];
																																									Columns[7] |= LS[37];
																																									for (CR[38] = AllN ^ (Rows[3] | Columns[8]); CR[38] != 0; CR[38] = (CR[38] - 1) & CR[38]) {
																																										LS[38] = (-CR[38]) & CR[38];
																																										Rows[3] |= LS[38];
																																										Columns[8] |= LS[38];
																																										CR[39] = AllN ^ (Rows[3] | Columns[9]);
																																										if (CR[39] != 0) {
																																											LS[39] = (-CR[39]) & CR[39];
																																											Columns[9] |= LS[39];
																																											for (CR[64] = AllN ^ (Rows[6] | Columns[4]); CR[64] != 0; CR[64] = (CR[64] - 1) & CR[64]) {
																																												LS[64] = (-CR[64]) & CR[64];
																																												Rows[6] |= LS[64];
																																												Columns[4] |= LS[64];
																																												for (CR[65] = AllN ^ (Rows[6] | Columns[5]); CR[65] != 0; CR[65] = (CR[65] - 1) & CR[65]) {
																																													LS[65] = (-CR[65]) & CR[65];
																																													Rows[6] |= LS[65];
																																													Columns[5] |= LS[65];
																																													for (CR[60] = AllN ^ (Rows[6] | Columns[0]); CR[60] != 0; CR[60] = (CR[60] - 1) & CR[60]) {
																																														LS[60] = (-CR[60]) & CR[60];
																																														Rows[6] |= LS[60];
																																														Columns[0] |= LS[60];
																																														for (CR[61] = AllN ^ (Rows[6] | Columns[1]); CR[61] != 0; CR[61] = (CR[61] - 1) & CR[61]) {
																																															LS[61] = (-CR[61]) & CR[61];
																																															Rows[6] |= LS[61];
																																															Columns[1] |= LS[61];
																																															for (CR[62] = AllN ^ (Rows[6] | Columns[2]); CR[62] != 0; CR[62] = (CR[62] - 1) & CR[62]) {
																																																LS[62] = (-CR[62]) & CR[62];
																																																Rows[6] |= LS[62];
																																																Columns[2] |= LS[62];
																																																for (CR[67] = AllN ^ (Rows[6] | Columns[7]); CR[67] != 0; CR[67] = (CR[67] - 1) & CR[67]) {
																																																	LS[67] = (-CR[67]) & CR[67];
																																																	Rows[6] |= LS[67];
																																																	Columns[7] |= LS[67];
																																																	for (CR[68] = AllN ^ (Rows[6] | Columns[8]); CR[68] != 0; CR[68] = (CR[68] - 1) & CR[68]) {
																																																		LS[68] = (-CR[68]) & CR[68];
																																																		Rows[6] |= LS[68];
																																																		Columns[8] |= LS[68];
																																																		CR[69] = AllN ^ (Rows[6] | Columns[9]);
																																																		if (CR[69] != 0) {
																																																			LS[69] = (-CR[69]) & CR[69];
																																																			Columns[9] |= LS[69];
																																																			for (CR[74] = AllN ^ (Rows[7] | Columns[4]); CR[74] != 0; CR[74] = (CR[74] - 1) & CR[74]) {
																																																				LS[74] = (-CR[74]) & CR[74];
																																																				Rows[7] |= LS[74];
																																																				Columns[4] |= LS[74];
																																																				for (CR[75] = AllN ^ (Rows[7] | Columns[5]); CR[75] != 0; CR[75] = (CR[75] - 1) & CR[75]) {
																																																					LS[75] = (-CR[75]) & CR[75];
																																																					Rows[7] |= LS[75];
																																																					Columns[5] |= LS[75];
																																																					for (CR[70] = AllN ^ (Rows[7] | Columns[0]); CR[70] != 0; CR[70] = (CR[70] - 1) & CR[70]) {
																																																						LS[70] = (-CR[70]) & CR[70];
																																																						Rows[7] |= LS[70];
																																																						Columns[0] |= LS[70];
																																																						for (CR[71] = AllN ^ (Rows[7] | Columns[1]); CR[71] != 0; CR[71] = (CR[71] - 1) & CR[71]) {
																																																							LS[71] = (-CR[71]) & CR[71];
																																																							Rows[7] |= LS[71];
																																																							Columns[1] |= LS[71];
																																																							for (CR[78] = AllN ^ (Rows[7] | Columns[8]); CR[78] != 0; CR[78] = (CR[78] - 1) & CR[78]) {
																																																								LS[78] = (-CR[78]) & CR[78];
																																																								Rows[7] |= LS[78];
																																																								Columns[8] |= LS[78];
																																																								for (CR[79] = AllN ^ (Rows[7] | Columns[9]); CR[79] != 0; CR[79] = (CR[79] - 1) & CR[79]) {
																																																									LS[79] = (-CR[79]) & CR[79];
																																																									Rows[7] |= LS[79];
																																																									Columns[9] |= LS[79];
																																																									for (CR[73] = AllN ^ (Rows[7] | Columns[3]); CR[73] != 0; CR[73] = (CR[73] - 1) & CR[73]) {
																																																										LS[73] = (-CR[73]) & CR[73];
																																																										Rows[7] |= LS[73];
																																																										Columns[3] |= LS[73];
																																																										CR[76] = AllN ^ (Rows[7] | Columns[6]);
																																																										if (CR[76] != 0) {
																																																											LS[76] = (-CR[76]) & CR[76];
																																																											Columns[6] |= LS[76];
																																																											for (CR[84] = AllN ^ (Rows[8] | Columns[4]); CR[84] != 0; CR[84] = (CR[84] - 1) & CR[84]) {
																																																												LS[84] = (-CR[84]) & CR[84];
																																																												Rows[8] |= LS[84];
																																																												Columns[4] |= LS[84];
																																																												CR[94] = AllN ^ (Rows[9] | Columns[4]);
																																																												if (CR[94] != 0) {
																																																													LS[94] = (-CR[94]) & CR[94];
																																																													Rows[9] |= LS[94];
																																																													for (CR[85] = AllN ^ (Rows[8] | Columns[5]); CR[85] != 0; CR[85] = (CR[85] - 1) & CR[85]) {
																																																														LS[85] = (-CR[85]) & CR[85];
																																																														Rows[8] |= LS[85];
																																																														Columns[5] |= LS[85];
																																																														CR[95] = AllN ^ (Rows[9] | Columns[5]);
																																																														if (CR[95] != 0) {
																																																															LS[95] = (-CR[95]) & CR[95];
																																																															Rows[9] |= LS[95];
																																																															for (CR[80] = AllN ^ (Rows[8] | Columns[0]); CR[80] != 0; CR[80] = (CR[80] - 1) & CR[80]) {
																																																																LS[80] = (-CR[80]) & CR[80];
																																																																Rows[8] |= LS[80];
																																																																Columns[0] |= LS[80];
																																																																for (CR[89] = AllN ^ (Rows[8] | Columns[9]); CR[89] != 0; CR[89] = (CR[89] - 1) & CR[89]) {
																																																																	LS[89] = (-CR[89]) & CR[89];
																																																																	Rows[8] |= LS[89];
																																																																	Columns[9] |= LS[89];
																																																																	for (CR[82] = AllN ^ (Rows[8] | Columns[2]); CR[82] != 0; CR[82] = (CR[82] - 1) & CR[82]) {
																																																																		LS[82] = (-CR[82]) & CR[82];
																																																																		Rows[8] |= LS[82];
																																																																		Columns[2] |= LS[82];
																																																																		for (CR[83] = AllN ^ (Rows[8] | Columns[3]); CR[83] != 0; CR[83] = (CR[83] - 1) & CR[83]) {
																																																																			LS[83] = (-CR[83]) & CR[83];
																																																																			Rows[8] |= LS[83];
																																																																			Columns[3] |= LS[83];
																																																																			for (CR[86] = AllN ^ (Rows[8] | Columns[6]); CR[86] != 0; CR[86] = (CR[86] - 1) & CR[86]) {
																																																																				LS[86] = (-CR[86]) & CR[86];
																																																																				Rows[8] |= LS[86];
																																																																				Columns[6] |= LS[86];
																																																																				CR[87] = AllN ^ (Rows[8] | Columns[7]);
																																																																				if (CR[87] != 0) {
																																																																					LS[87] = (-CR[87]) & CR[87];
																																																																					Columns[7] |= LS[87];
																																																																					for (CR[91] = AllN ^ (Rows[9] | Columns[1]); CR[91] != 0; CR[91] = (CR[91] - 1) & CR[91]) {
																																																																						LS[91] = (-CR[91]) & CR[91];
																																																																						Rows[9] |= LS[91];
																																																																						Columns[1] |= LS[91];
																																																																						for (CR[92] = AllN ^ (Rows[9] | Columns[2]); CR[92] != 0; CR[92] = (CR[92] - 1) & CR[92]) {
																																																																							LS[92] = (-CR[92]) & CR[92];
																																																																							Rows[9] |= LS[92];
																																																																							Columns[2] |= LS[92];
																																																																							for (CR[93] = AllN ^ (Rows[9] | Columns[3]); CR[93] != 0; CR[93] = (CR[93] - 1) & CR[93]) {
																																																																								LS[93] = (-CR[93]) & CR[93];
																																																																								Rows[9] |= LS[93];
																																																																								Columns[3] |= LS[93];
																																																																								for (CR[96] = AllN ^ (Rows[9] | Columns[6]); CR[96] != 0; CR[96] = (CR[96] - 1) & CR[96]) {
																																																																									LS[96] = (-CR[96]) & CR[96];
																																																																									Rows[9] |= LS[96];
																																																																									Columns[6] |= LS[96];
																																																																									for (CR[97] = AllN ^ (Rows[9] | Columns[7]); CR[97] != 0; CR[97] = (CR[97] - 1) & CR[97]) {
																																																																										LS[97] = (-CR[97]) & CR[97];
																																																																										Rows[9] |= LS[97];
																																																																										Columns[7] |= LS[97];
																																																																										CR[98] = AllN ^ (Rows[9] | Columns[8]);
																																																																										if (CR[98] != 0) {
																																																																											LS[98] = (-CR[98]) & CR[98];
																																																																											Columns[8] |= LS[98];
																																																																											for (CR[40] = AllN ^ (Rows[4] | Columns[0]); CR[40] != 0; CR[40] = (CR[40] - 1) & CR[40]) {
																																																																												LS[40] = (-CR[40]) & CR[40];
																																																																												Rows[4] |= LS[40];
																																																																												Columns[0] |= LS[40];
																																																																												CR[50] = AllN ^ (Rows[5] | Columns[0]);
																																																																												if (CR[50] != 0) {
																																																																													LS[50] = (-CR[50]) & CR[50];
																																																																													Rows[5] |= LS[50];
																																																																													for (CR[41] = AllN ^ (Rows[4] | Columns[1]); CR[41] != 0; CR[41] = (CR[41] - 1) & CR[41]) {
																																																																														LS[41] = (-CR[41]) & CR[41];
																																																																														Rows[4] |= LS[41];
																																																																														Columns[1] |= LS[41];
																																																																														CR[51] = AllN ^ (Rows[5] | Columns[1]);
																																																																														if (CR[51] != 0) {
																																																																															LS[51] = (-CR[51]) & CR[51];
																																																																															Rows[5] |= LS[51];
																																																																															for (CR[42] = AllN ^ (Rows[4] | Columns[2]); CR[42] != 0; CR[42] = (CR[42] - 1) & CR[42]) {
																																																																																LS[42] = (-CR[42]) & CR[42];
																																																																																Rows[4] |= LS[42];
																																																																																Columns[2] |= LS[42];
																																																																																CR[52] = AllN ^ (Rows[5] | Columns[2]);
																																																																																if (CR[52] != 0) {
																																																																																	LS[52] = (-CR[52]) & CR[52];
																																																																																	Rows[5] |= LS[52];
																																																																																	for (CR[43] = AllN ^ (Rows[4] | Columns[3]); CR[43] != 0; CR[43] = (CR[43] - 1) & CR[43]) {
																																																																																		LS[43] = (-CR[43]) & CR[43];
																																																																																		Rows[4] |= LS[43];
																																																																																		Columns[3] |= LS[43];
																																																																																		CR[53] = AllN ^ (Rows[5] | Columns[3]);
																																																																																		if (CR[53] != 0) {
																																																																																			LS[53] = (-CR[53]) & CR[53];
																																																																																			Rows[5] |= LS[53];
																																																																																			for (CR[46] = AllN ^ (Rows[4] | Columns[6]); CR[46] != 0; CR[46] = (CR[46] - 1) & CR[46]) {
																																																																																				LS[46] = (-CR[46]) & CR[46];
																																																																																				Rows[4] |= LS[46];
																																																																																				Columns[6] |= LS[46];
																																																																																				CR[56] = AllN ^ (Rows[5] | Columns[6]);
																																																																																				if (CR[56] != 0) {
																																																																																					LS[56] = (-CR[56]) & CR[56];
																																																																																					Rows[5] |= LS[56];
																																																																																					for (CR[47] = AllN ^ (Rows[4] | Columns[7]); CR[47] != 0; CR[47] = (CR[47] - 1) & CR[47]) {
																																																																																						LS[47] = (-CR[47]) & CR[47];
																																																																																						Rows[4] |= LS[47];
																																																																																						Columns[7] |= LS[47];
																																																																																						CR[57] = AllN ^ (Rows[5] | Columns[7]);
																																																																																						if (CR[57] != 0) {
																																																																																							LS[57] = (-CR[57]) & CR[57];
																																																																																							Rows[5] |= LS[57];
																																																																																							for (CR[48] = AllN ^ (Rows[4] | Columns[8]); CR[48] != 0; CR[48] = (CR[48] - 1) & CR[48]) {
																																																																																								LS[48] = (-CR[48]) & CR[48];
																																																																																								Rows[4] |= LS[48];
																																																																																								Columns[8] |= LS[48];
																																																																																								CR[49] = AllN ^ (Rows[4] | Columns[9]);
																																																																																								if (CR[49] != 0) {
																																																																																									LS[49] = (-CR[49]) & CR[49];
																																																																																									Columns[9] |= LS[49];
																																																																																									CR[58] = AllN ^ (Rows[5] | Columns[8]);
																																																																																									if (CR[58] != 0) {
																																																																																										LS[58] = (-CR[58]) & CR[58];
																																																																																										Rows[5] |= LS[58];
																																																																																										CR[59] = AllN ^ (Rows[5] | Columns[9]);
																																																																																										if (CR[59] != 0) {
																																																																																											LS[59] = (-CR[59]) & CR[59];
																																																																																											SquaresCnt++;
																																																																																											if (SquaresCnt % 10000000 == 0) {
																																																																																												cout << "Squares found: " << SquaresCnt << endl;
																																																																																											}
																																																																																										}
																																																																																										Rows[5] ^= LS[58];
																																																																																									}
																																																																																									Columns[9] ^= LS[49];
																																																																																								}
																																																																																								Rows[4] ^= LS[48];
																																																																																								Columns[8] ^= LS[48];
																																																																																							}
																																																																																							Rows[5] ^= LS[57];
																																																																																						}
																																																																																						Rows[4] ^= LS[47];
																																																																																						Columns[7] ^= LS[47];
																																																																																					}
																																																																																					Rows[5] ^= LS[56];
																																																																																				}
																																																																																				Rows[4] ^= LS[46];
																																																																																				Columns[6] ^= LS[46];
																																																																																			}
																																																																																			Rows[5] ^= LS[53];
																																																																																		}
																																																																																		Rows[4] ^= LS[43];
																																																																																		Columns[3] ^= LS[43];
																																																																																	}
																																																																																	Rows[5] ^= LS[52];
																																																																																}
																																																																																Rows[4] ^= LS[42];
																																																																																Columns[2] ^= LS[42];
																																																																															}
																																																																															Rows[5] ^= LS[51];
																																																																														}
																																																																														Rows[4] ^= LS[41];
																																																																														Columns[1] ^= LS[41];
																																																																													}
																																																																													Rows[5] ^= LS[50];
																																																																												}
																																																																												Rows[4] ^= LS[40];
																																																																												Columns[0] ^= LS[40];
																																																																											}
																																																																											Columns[8] ^= LS[98];
																																																																										}
																																																																										Rows[9] ^= LS[97];
																																																																										Columns[7] ^= LS[97];
																																																																									}
																																																																									Rows[9] ^= LS[96];
																																																																									Columns[6] ^= LS[96];
																																																																								}
																																																																								Rows[9] ^= LS[93];
																																																																								Columns[3] ^= LS[93];
																																																																							}
																																																																							Rows[9] ^= LS[92];
																																																																							Columns[2] ^= LS[92];
																																																																						}
																																																																						Rows[9] ^= LS[91];
																																																																						Columns[1] ^= LS[91];
																																																																					}
																																																																					Columns[7] ^= LS[87];
																																																																				}
																																																																				Rows[8] ^= LS[86];
																																																																				Columns[6] ^= LS[86];
																																																																			}
																																																																			Rows[8] ^= LS[83];
																																																																			Columns[3] ^= LS[83];
																																																																		}
																																																																		Rows[8] ^= LS[82];
																																																																		Columns[2] ^= LS[82];
																																																																	}
																																																																	Rows[8] ^= LS[89];
																																																																	Columns[9] ^= LS[89];
																																																																}
																																																																Rows[8] ^= LS[80];
																																																																Columns[0] ^= LS[80];
																																																															}
																																																															Rows[9] ^= LS[95];
																																																														}
																																																														Rows[8] ^= LS[85];
																																																														Columns[5] ^= LS[85];
																																																													}
																																																													Rows[9] ^= LS[94];
																																																												}
																																																												Rows[8] ^= LS[84];
																																																												Columns[4] ^= LS[84];
																																																											}
																																																											Columns[6] ^= LS[76];
																																																										}
																																																										Rows[7] ^= LS[73];
																																																										Columns[3] ^= LS[73];
																																																									}
																																																									Rows[7] ^= LS[79];
																																																									Columns[9] ^= LS[79];
																																																								}
																																																								Rows[7] ^= LS[78];
																																																								Columns[8] ^= LS[78];
																																																							}
																																																							Rows[7] ^= LS[71];
																																																							Columns[1] ^= LS[71];
																																																						}
																																																						Rows[7] ^= LS[70];
																																																						Columns[0] ^= LS[70];
																																																					}
																																																					Rows[7] ^= LS[75];
																																																					Columns[5] ^= LS[75];
																																																				}
																																																				Rows[7] ^= LS[74];
																																																				Columns[4] ^= LS[74];
																																																			}
																																																			Columns[9] ^= LS[69];
																																																		}
																																																		Rows[6] ^= LS[68];
																																																		Columns[8] ^= LS[68];
																																																	}
																																																	Rows[6] ^= LS[67];
																																																	Columns[7] ^= LS[67];
																																																}
																																																Rows[6] ^= LS[62];
																																																Columns[2] ^= LS[62];
																																															}
																																															Rows[6] ^= LS[61];
																																															Columns[1] ^= LS[61];
																																														}
																																														Rows[6] ^= LS[60];
																																														Columns[0] ^= LS[60];
																																													}
																																													Rows[6] ^= LS[65];
																																													Columns[5] ^= LS[65];
																																												}
																																												Rows[6] ^= LS[64];
																																												Columns[4] ^= LS[64];
																																											}
																																											Columns[9] ^= LS[39];
																																										}
																																										Rows[3] ^= LS[38];
																																										Columns[8] ^= LS[38];
																																									}
																																									Rows[3] ^= LS[37];
																																									Columns[7] ^= LS[37];
																																								}
																																								Rows[3] ^= LS[32];
																																								Columns[2] ^= LS[32];
																																							}
																																							Rows[3] ^= LS[31];
																																							Columns[1] ^= LS[31];
																																						}
																																						Rows[3] ^= LS[30];
																																						Columns[0] ^= LS[30];
																																					}
																																					Rows[3] ^= LS[35];
																																					Columns[5] ^= LS[35];
																																				}
																																				Rows[3] ^= LS[34];
																																				Columns[4] ^= LS[34];
																																			}
																																			Columns[9] ^= LS[29];
																																		}
																																		Rows[2] ^= LS[28];
																																		Columns[8] ^= LS[28];
																																	}
																																	Rows[2] ^= LS[21];
																																	Columns[1] ^= LS[21];
																																}
																																Rows[2] ^= LS[20];
																																Columns[0] ^= LS[20];
																															}
																															Rows[2] ^= LS[26];
																															Columns[6] ^= LS[26];
																														}
																														Rows[2] ^= LS[25];
																														Columns[5] ^= LS[25];
																													}
																													Rows[2] ^= LS[24];
																													Columns[4] ^= LS[24];
																												}
																												Rows[2] ^= LS[23];
																												Columns[3] ^= LS[23];
																											}
																											Columns[9] ^= LS[19];
																										}
																										Rows[1] ^= LS[10];
																										Columns[0] ^= LS[10];
																									}
																									Rows[1] ^= LS[17];
																									Columns[7] ^= LS[17];
																								}
																								Rows[1] ^= LS[16];
																								Columns[6] ^= LS[16];
																							}
																							Rows[1] ^= LS[15];
																							Columns[5] ^= LS[15];
																						}
																						Rows[1] ^= LS[14];
																						Columns[4] ^= LS[14];
																					}
																					Rows[1] ^= LS[13];
																					Columns[3] ^= LS[13];
																				}
																				Rows[1] ^= LS[12];
																				Columns[2] ^= LS[12];
																			}
																			Columns[8] ^= LS[8];
																		}
																		Rows[0] ^= LS[7];
																		Columns[7] ^= LS[7];
																	}
																	Rows[0] ^= LS[6];
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
												Rows[0] ^= LS[1];
												Columns[1] ^= LS[1];
											}
											Rows[0] ^= LS[9];
											Columns[9] ^= LS[9];
										}
										Rows[1] ^= LS[18];
										Columns[8] ^= LS[18];
										AD ^= LS[18];
									}
									Rows[2] ^= LS[27];
									Columns[7] ^= LS[27];
									AD ^= LS[27];
								}
								Rows[3] ^= LS[36];
								Columns[6] ^= LS[36];
								AD ^= LS[36];
							}
							Rows[4] ^= LS[45];
							Columns[5] ^= LS[45];
							AD ^= LS[45];
						}
						Rows[5] ^= LS[54];
						Columns[4] ^= LS[54];
						AD ^= LS[54];
					}
					Rows[6] ^= LS[63];
					Columns[3] ^= LS[63];
					AD ^= LS[63];
				}
				Rows[7] ^= LS[72];
				Columns[2] ^= LS[72];
				AD ^= LS[72];
			}
			Rows[8] ^= LS[81];
			Columns[1] ^= LS[81];
			AD ^= LS[81];
		}
		Rows[9] ^= LS[90];
		Columns[0] ^= LS[90];
		AD ^= LS[90];
	}
}
