#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <omp.h>
static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
using namespace std;
const int N = 9;
int cur_cnt = 0;
/*
const uint32_t MaxN = 1 << N;
const uint32_t AllN = MaxN - 1;
int32_t LS[N*N];
int32_t CR[N*N];

int32_t * OC;
uint64_t SquaresCnt = 0;
uint32_t Columns[N], Rows[N], MD, AD;
*/
int* trf = new int[512];
__int64 ProcFreq = 3.5e9;
__int64 _StartProf, _StopProf;
void StartProf()
{
	_StartProf = __rdtsc();
}
void StopProf()
{
	_StopProf = __rdtsc();
}
__int64 ProfResult()
{
	return _StopProf - _StartProf;
}
/*
void Reset()
{
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
	OC[1 << 0] |= 1 << 0;
	MD |= LS[0];
	AD |= LS[N - 1];
}
*/
//void Generate();

static inline bool is_SODLS(int32_t LS[N*N]) {	
	bool res = true;
	int32_t NLS[N*N];
	for (int i = 0; i < N*N; i++) {
		NLS[i] = trf[LS[i]];
	}
	int32_t TLS[N*N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			TLS[i*N + j] = NLS[j*N + i];
		}
	}
	vector<int> OCT(N*N);
	for (int i = 0; i < N*N; i++) {
		int ind = NLS[i] * N + TLS[i];
		OCT[ind]++;
		if (OCT[ind]>1) {
			res = false;
			break;
		}
	}
	ofstream out;
	out.open("D:\\LSTests\\Eduard\\SODLS9.txt", ios::app);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			out << NLS[i*N + j] << " ";
		}
		out << endl;
	}
	out << endl;
	
	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << TLS[i*N + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	res == true ? cout << "SODLS" : cout << "NOT SODLS";
	cout << endl;*/
	return res;
}


vector<vector<int>> gen_prefixes() {

	const uint32_t MaxN = 1 << N;
	const uint32_t AllN = MaxN - 1;
	int32_t LS[N*N];
	int32_t CR[N*N];	
	int32_t * OC;
	OC = new int32_t[1 << 9 + 1];
	for (int i = 0; i<N; i++) {
		OC[1 << i] = 0;
	}
	uint64_t SquaresCnt = 0;
	uint32_t Columns[N], Rows[N], MD, AD;
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
	OC[1 << 0] |= 1 << 0;
	MD |= LS[0];
	AD |= LS[N - 1];
	int cnt = 0;
	vector<vector<int>> res;
	for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); ((CR[10] != 0)); CR[10] = (CR[10] & (CR[10] - 1))) {
		LS[10] = (CR[10] & (-CR[10]));
		Rows[1] |= LS[10];
		Columns[1] |= LS[10];
		MD |= LS[10];
		OC[LS[10]] |= LS[10];
		for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); ((CR[20] != 0)); CR[20] = (CR[20] & (CR[20] - 1))) {
			LS[20] = (CR[20] & (-CR[20]));
			Rows[2] |= LS[20];
			Columns[2] |= LS[20];
			MD |= LS[20];
			OC[LS[20]] |= LS[20];
			for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); ((CR[30] != 0)); CR[30] = (CR[30] & (CR[30] - 1))) {
				LS[30] = (CR[30] & (-CR[30]));
				Rows[3] |= LS[30];
				Columns[3] |= LS[30];
				MD |= LS[30];
				OC[LS[30]] |= LS[30];
				for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); ((CR[40] != 0)); CR[40] = (CR[40] & (CR[40] - 1))) {
					LS[40] = (CR[40] & (-CR[40]));
					Rows[4] |= LS[40];
					Columns[4] |= LS[40];
					MD |= LS[40];
					AD |= LS[40];
					OC[LS[40]] |= LS[40];
					for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); ((CR[50] != 0)); CR[50] = (CR[50] & (CR[50] - 1))) {
						LS[50] = (CR[50] & (-CR[50]));
						Rows[5] |= LS[50];
						Columns[5] |= LS[50];
						MD |= LS[50];
						OC[LS[50]] |= LS[50];
						for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); ((CR[60] != 0)); CR[60] = (CR[60] & (CR[60] - 1))) {
							LS[60] = (CR[60] & (-CR[60]));
							Rows[6] |= LS[60];
							Columns[6] |= LS[60];
							MD |= LS[60];
							OC[LS[60]] |= LS[60];
							for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); ((CR[70] != 0)); CR[70] = (CR[70] & (CR[70] - 1))) {
								LS[70] = (CR[70] & (-CR[70]));
								Rows[7] |= LS[70];
								Columns[7] |= LS[70];
								MD |= LS[70];
								OC[LS[70]] |= LS[70];
								CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
								if ((CR[80] != 0)) {
									LS[80] = (CR[80] & (-CR[80]));
									cnt++;
									vector<int> t;
									t.push_back(LS[10]);
									t.push_back(LS[20]);
									t.push_back(LS[30]);
									t.push_back(LS[40]);
									t.push_back(LS[50]);
									t.push_back(LS[60]);
									t.push_back(LS[70]);
									t.push_back(LS[80]);
									res.push_back(t);

								}
								Rows[7] ^= LS[70];
								Columns[7] ^= LS[70];
								MD ^= LS[70];
								OC[LS[70]] ^= LS[70];
							}
							Rows[6] ^= LS[60];
							Columns[6] ^= LS[60];
							MD ^= LS[60];
							OC[LS[60]] ^= LS[60];
						}
						Rows[5] ^= LS[50];
						Columns[5] ^= LS[50];
						MD ^= LS[50];
						OC[LS[50]] ^= LS[50];
					}
					Rows[4] ^= LS[40];
					Columns[4] ^= LS[40];
					MD ^= LS[40];
					AD ^= LS[40];
					OC[LS[40]] ^= LS[40];
				}
				Rows[3] ^= LS[30];
				Columns[3] ^= LS[30];
				MD ^= LS[30];
				OC[LS[30]] ^= LS[30];
			}
			Rows[2] ^= LS[20];
			Columns[2] ^= LS[20];
			MD ^= LS[20];
			OC[LS[20]] ^= LS[20];
		}
		Rows[1] ^= LS[10];
		Columns[1] ^= LS[10];
		MD ^= LS[10];
		OC[LS[10]] ^= LS[10];
	}
	cout << "Couunt " << cnt << endl;
	return res;
}


int process_prefix(vector<int> pr) {
	const uint32_t MaxN = 1 << N;
	const uint32_t AllN = MaxN - 1;
	int32_t LS[N*N];
	int32_t CR[N*N];
	
	int32_t * OC;
	OC = new int32_t[1 << 9 + 1];
	for (int i = 0; i<N; i++) {
		OC[1 << i] = 0;
	}
	uint64_t SquaresCnt = 0;
	uint32_t Columns[N], Rows[N], MD, AD;
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
	OC[1 << 0] |= 1 << 0;
	MD |= LS[0];
	AD |= LS[N - 1];

	int pr_cnt = 0;
	//for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); ((CR[10] != 0)); CR[10] = (CR[10] & (CR[10] - 1))) {
	{
		LS[10] = pr[0];
		Rows[1] |= LS[10];
		Columns[1] |= LS[10];
		MD |= LS[10];
		OC[LS[10]] |= LS[10];
		//	for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); ((CR[20] != 0)); CR[20] = (CR[20] & (CR[20] - 1))) {
			//	LS[20] = (CR[20] & (-CR[20]));
		{
			LS[20] = pr[1];
			Rows[2] |= LS[20];
			Columns[2] |= LS[20];
			MD |= LS[20];
			OC[LS[20]] |= LS[20];
			//for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); ((CR[30] != 0)); CR[30] = (CR[30] & (CR[30] - 1))) {
			//LS[30] = (CR[30] & (-CR[30]));
			{
				LS[30] = pr[2];
				Rows[3] |= LS[30];
				Columns[3] |= LS[30];
				MD |= LS[30];
				OC[LS[30]] |= LS[30];
				//for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); ((CR[40] != 0)); CR[40] = (CR[40] & (CR[40] - 1))) {
					//LS[40] = (CR[40] & (-CR[40]));
				{
					LS[40] = pr[3];
					Rows[4] |= LS[40];
					Columns[4] |= LS[40];
					MD |= LS[40];
					AD |= LS[40];
					OC[LS[40]] |= LS[40];
					//for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); ((CR[50] != 0)); CR[50] = (CR[50] & (CR[50] - 1))) {
						//LS[50] = (CR[50] & (-CR[50]));
					{
						LS[50] = pr[4];
						Rows[5] |= LS[50];
						Columns[5] |= LS[50];
						MD |= LS[50];
						OC[LS[50]] |= LS[50];
						//for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); ((CR[60] != 0)); CR[60] = (CR[60] & (CR[60] - 1))) {
							//LS[60] = (CR[60] & (-CR[60]));
						{
							LS[60] = pr[5];
							Rows[6] |= LS[60];
							Columns[6] |= LS[60];
							MD |= LS[60];
							OC[LS[60]] |= LS[60];
							//for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); ((CR[70] != 0)); CR[70] = (CR[70] & (CR[70] - 1))) {
							//	LS[70] = (CR[70] & (-CR[70]));
							{
								LS[70] = pr[6];
								Rows[7] |= LS[70];
								Columns[7] |= LS[70];
								MD |= LS[70];
								OC[LS[70]] |= LS[70];
								CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
								//if ((CR[80] != 0)) {
									//LS[80] = (CR[80] & (-CR[80]));
								{
									LS[80] = pr[7];
									Rows[8] |= LS[80];
									Columns[8] |= LS[80];
									OC[LS[80]] |= LS[80];

									for (CR[9] = AllN ^ (Rows[1] | Columns[0] | OC[LS[1]]); ((CR[9] != 0)); CR[9] = (CR[9] & (CR[9] - 1))) {
										LS[9] = (CR[9] & (-CR[9]));
										Rows[1] |= LS[9];
										Columns[0] |= LS[9];
										OC[LS[9]] |= LS[1];
										OC[LS[1]] |= LS[9];
										for (CR[18] = AllN ^ (Rows[2] | Columns[0] | OC[LS[2]]); ((CR[18] != 0)); CR[18] = (CR[18] & (CR[18] - 1))) {
											LS[18] = (CR[18] & (-CR[18]));
											Rows[2] |= LS[18];
											Columns[0] |= LS[18];
											OC[LS[18]] |= LS[2];
											OC[LS[2]] |= LS[18];
											for (CR[27] = AllN ^ (Rows[3] | Columns[0] | OC[LS[3]]); ((CR[27] != 0)); CR[27] = (CR[27] & (CR[27] - 1))) {
												LS[27] = (CR[27] & (-CR[27]));
												Rows[3] |= LS[27];
												Columns[0] |= LS[27];
												OC[LS[27]] |= LS[3];
												OC[LS[3]] |= LS[27];
												for (CR[36] = AllN ^ (Rows[4] | Columns[0] | OC[LS[4]]); ((CR[36] != 0)); CR[36] = (CR[36] & (CR[36] - 1))) {
													LS[36] = (CR[36] & (-CR[36]));
													Rows[4] |= LS[36];
													Columns[0] |= LS[36];
													OC[LS[36]] |= LS[4];
													OC[LS[4]] |= LS[36];
													for (CR[45] = AllN ^ (Rows[5] | Columns[0] | OC[LS[5]]); ((CR[45] != 0)); CR[45] = (CR[45] & (CR[45] - 1))) {
														LS[45] = (CR[45] & (-CR[45]));
														Rows[5] |= LS[45];
														Columns[0] |= LS[45];
														OC[LS[45]] |= LS[5];
														OC[LS[5]] |= LS[45];
														for (CR[54] = AllN ^ (Rows[6] | Columns[0] | OC[LS[6]]); ((CR[54] != 0)); CR[54] = (CR[54] & (CR[54] - 1))) {
															LS[54] = (CR[54] & (-CR[54]));
															Rows[6] |= LS[54];
															Columns[0] |= LS[54];
															OC[LS[54]] |= LS[6];
															OC[LS[6]] |= LS[54];
															for (CR[63] = AllN ^ (Rows[7] | Columns[0] | OC[LS[7]]); ((CR[63] != 0)); CR[63] = (CR[63] & (CR[63] - 1))) {
																LS[63] = (CR[63] & (-CR[63]));
																Rows[7] |= LS[63];
																Columns[0] |= LS[63];
																OC[LS[63]] |= LS[7];
																OC[LS[7]] |= LS[63];
																CR[72] = AllN ^ (Rows[8] | Columns[0] | OC[LS[8]] | AD);
																if ((CR[72] != 0)) {
																	LS[72] = (CR[72] & (-CR[72]));
																	Rows[8] |= LS[72];
																	AD |= LS[72];
																	OC[LS[72]] |= LS[8];
																	OC[LS[8]] |= LS[72];
																	for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); ((CR[16] != 0)); CR[16] = (CR[16] & (CR[16] - 1))) {
																		LS[16] = (CR[16] & (-CR[16]));
																		Rows[1] |= LS[16];
																		Columns[7] |= LS[16];
																		AD |= LS[16];
																		for (CR[64] = AllN ^ (Rows[7] | Columns[1] | OC[LS[16]] | AD); ((CR[64] != 0)); CR[64] = (CR[64] & (CR[64] - 1))) {
																			LS[64] = (CR[64] & (-CR[64]));
																			Rows[7] |= LS[64];
																			Columns[1] |= LS[64];
																			AD |= LS[64];
																			OC[LS[64]] |= LS[16];
																			OC[LS[16]] |= LS[64];
																			for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); ((CR[24] != 0)); CR[24] = (CR[24] & (CR[24] - 1))) {
																				LS[24] = (CR[24] & (-CR[24]));
																				Rows[2] |= LS[24];
																				Columns[6] |= LS[24];
																				AD |= LS[24];
																				for (CR[56] = AllN ^ (Rows[6] | Columns[2] | OC[LS[24]] | AD); ((CR[56] != 0)); CR[56] = (CR[56] & (CR[56] - 1))) {
																					LS[56] = (CR[56] & (-CR[56]));
																					Rows[6] |= LS[56];
																					Columns[2] |= LS[56];
																					AD |= LS[56];
																					OC[LS[56]] |= LS[24];
																					OC[LS[24]] |= LS[56];
																					for (CR[32] = AllN ^ (Rows[3] | Columns[5] | AD); ((CR[32] != 0)); CR[32] = (CR[32] & (CR[32] - 1))) {
																						LS[32] = (CR[32] & (-CR[32]));
																						Rows[3] |= LS[32];
																						Columns[5] |= LS[32];
																						AD |= LS[32];
																						CR[48] = AllN ^ (Rows[5] | Columns[3] | OC[LS[32]] | AD);
																						if ((CR[48] != 0)) {
																							LS[48] = (CR[48] & (-CR[48]));
																							Rows[5] |= LS[48];
																							Columns[3] |= LS[48];
																							OC[LS[48]] |= LS[32];
																							OC[LS[32]] |= LS[48];
																							for (CR[11] = AllN ^ (Rows[1] | Columns[2]); ((CR[11] != 0)); CR[11] = (CR[11] & (CR[11] - 1))) {
																								LS[11] = (CR[11] & (-CR[11]));
																								Rows[1] |= LS[11];
																								Columns[2] |= LS[11];
																								for (CR[19] = AllN ^ (Rows[2] | Columns[1] | OC[LS[11]]); ((CR[19] != 0)); CR[19] = (CR[19] & (CR[19] - 1))) {
																									LS[19] = (CR[19] & (-CR[19]));
																									Rows[2] |= LS[19];
																									Columns[1] |= LS[19];
																									OC[LS[19]] |= LS[11];
																									OC[LS[11]] |= LS[19];
																									for (CR[12] = AllN ^ (Rows[1] | Columns[3]); ((CR[12] != 0)); CR[12] = (CR[12] & (CR[12] - 1))) {
																										LS[12] = (CR[12] & (-CR[12]));
																										Rows[1] |= LS[12];
																										Columns[3] |= LS[12];
																										for (CR[28] = AllN ^ (Rows[3] | Columns[1] | OC[LS[12]]); ((CR[28] != 0)); CR[28] = (CR[28] & (CR[28] - 1))) {
																											LS[28] = (CR[28] & (-CR[28]));
																											Rows[3] |= LS[28];
																											Columns[1] |= LS[28];
																											OC[LS[28]] |= LS[12];
																											OC[LS[12]] |= LS[28];
																											for (CR[14] = AllN ^ (Rows[1] | Columns[5]); ((CR[14] != 0)); CR[14] = (CR[14] & (CR[14] - 1))) {
																												LS[14] = (CR[14] & (-CR[14]));
																												Rows[1] |= LS[14];
																												Columns[5] |= LS[14];
																												for (CR[46] = AllN ^ (Rows[5] | Columns[1] | OC[LS[14]]); ((CR[46] != 0)); CR[46] = (CR[46] & (CR[46] - 1))) {
																													LS[46] = (CR[46] & (-CR[46]));
																													Rows[5] |= LS[46];
																													Columns[1] |= LS[46];
																													OC[LS[46]] |= LS[14];
																													OC[LS[14]] |= LS[46];
																													for (CR[15] = AllN ^ (Rows[1] | Columns[6]); ((CR[15] != 0)); CR[15] = (CR[15] & (CR[15] - 1))) {
																														LS[15] = (CR[15] & (-CR[15]));
																														Rows[1] |= LS[15];
																														Columns[6] |= LS[15];
																														for (CR[55] = AllN ^ (Rows[6] | Columns[1] | OC[LS[15]]); ((CR[55] != 0)); CR[55] = (CR[55] & (CR[55] - 1))) {
																															LS[55] = (CR[55] & (-CR[55]));
																															Rows[6] |= LS[55];
																															Columns[1] |= LS[55];
																															OC[LS[55]] |= LS[15];
																															OC[LS[15]] |= LS[55];
																															for (CR[13] = AllN ^ (Rows[1] | Columns[4]); ((CR[13] != 0)); CR[13] = (CR[13] & (CR[13] - 1))) {
																																LS[13] = (CR[13] & (-CR[13]));
																																Rows[1] |= LS[13];
																																Columns[4] |= LS[13];
																																for (CR[37] = AllN ^ (Rows[4] | Columns[1] | OC[LS[13]]); ((CR[37] != 0)); CR[37] = (CR[37] & (CR[37] - 1))) {
																																	LS[37] = (CR[37] & (-CR[37]));
																																	Rows[4] |= LS[37];
																																	Columns[1] |= LS[37];
																																	OC[LS[37]] |= LS[13];
																																	OC[LS[13]] |= LS[37];
																																	CR[17] = AllN ^ (Rows[1] | Columns[8]);
																																	if ((CR[17] != 0)) {
																																		LS[17] = (CR[17] & (-CR[17]));
																																		Columns[8] |= LS[17];
																																		CR[73] = AllN ^ (Rows[8] | Columns[1] | OC[LS[17]]);
																																		if ((CR[73] != 0)) {
																																			LS[73] = (CR[73] & (-CR[73]));
																																			Rows[8] |= LS[73];
																																			OC[LS[73]] |= LS[17];
																																			OC[LS[17]] |= LS[73];
																																			for (CR[21] = AllN ^ (Rows[2] | Columns[3]); ((CR[21] != 0)); CR[21] = (CR[21] & (CR[21] - 1))) {
																																				LS[21] = (CR[21] & (-CR[21]));
																																				Rows[2] |= LS[21];
																																				Columns[3] |= LS[21];
																																				for (CR[29] = AllN ^ (Rows[3] | Columns[2] | OC[LS[21]]); ((CR[29] != 0)); CR[29] = (CR[29] & (CR[29] - 1))) {
																																					LS[29] = (CR[29] & (-CR[29]));
																																					Rows[3] |= LS[29];
																																					Columns[2] |= LS[29];
																																					OC[LS[29]] |= LS[21];
																																					OC[LS[21]] |= LS[29];
																																					for (CR[23] = AllN ^ (Rows[2] | Columns[5]); ((CR[23] != 0)); CR[23] = (CR[23] & (CR[23] - 1))) {
																																						LS[23] = (CR[23] & (-CR[23]));
																																						Rows[2] |= LS[23];
																																						Columns[5] |= LS[23];
																																						for (CR[47] = AllN ^ (Rows[5] | Columns[2] | OC[LS[23]]); ((CR[47] != 0)); CR[47] = (CR[47] & (CR[47] - 1))) {
																																							LS[47] = (CR[47] & (-CR[47]));
																																							Rows[5] |= LS[47];
																																							Columns[2] |= LS[47];
																																							OC[LS[47]] |= LS[23];
																																							OC[LS[23]] |= LS[47];
																																							for (CR[22] = AllN ^ (Rows[2] | Columns[4]); ((CR[22] != 0)); CR[22] = (CR[22] & (CR[22] - 1))) {
																																								LS[22] = (CR[22] & (-CR[22]));
																																								Rows[2] |= LS[22];
																																								Columns[4] |= LS[22];
																																								for (CR[38] = AllN ^ (Rows[4] | Columns[2] | OC[LS[22]]); ((CR[38] != 0)); CR[38] = (CR[38] & (CR[38] - 1))) {
																																									LS[38] = (CR[38] & (-CR[38]));
																																									Rows[4] |= LS[38];
																																									Columns[2] |= LS[38];
																																									OC[LS[38]] |= LS[22];
																																									OC[LS[22]] |= LS[38];
																																									for (CR[25] = AllN ^ (Rows[2] | Columns[7]); ((CR[25] != 0)); CR[25] = (CR[25] & (CR[25] - 1))) {
																																										LS[25] = (CR[25] & (-CR[25]));
																																										Rows[2] |= LS[25];
																																										Columns[7] |= LS[25];
																																										for (CR[65] = AllN ^ (Rows[7] | Columns[2] | OC[LS[25]]); ((CR[65] != 0)); CR[65] = (CR[65] & (CR[65] - 1))) {
																																											LS[65] = (CR[65] & (-CR[65]));
																																											Rows[7] |= LS[65];
																																											Columns[2] |= LS[65];
																																											OC[LS[65]] |= LS[25];
																																											OC[LS[25]] |= LS[65];
																																											CR[26] = AllN ^ (Rows[2] | Columns[8]);
																																											if ((CR[26] != 0)) {
																																												LS[26] = (CR[26] & (-CR[26]));
																																												Columns[8] |= LS[26];
																																												CR[74] = AllN ^ (Rows[8] | Columns[2] | OC[LS[26]]);
																																												if ((CR[74] != 0)) {
																																													LS[74] = (CR[74] & (-CR[74]));
																																													Rows[8] |= LS[74];
																																													OC[LS[74]] |= LS[26];
																																													OC[LS[26]] |= LS[74];
																																													for (CR[31] = AllN ^ (Rows[3] | Columns[4]); ((CR[31] != 0)); CR[31] = (CR[31] & (CR[31] - 1))) {
																																														LS[31] = (CR[31] & (-CR[31]));
																																														Rows[3] |= LS[31];
																																														Columns[4] |= LS[31];
																																														for (CR[39] = AllN ^ (Rows[4] | Columns[3] | OC[LS[31]]); ((CR[39] != 0)); CR[39] = (CR[39] & (CR[39] - 1))) {
																																															LS[39] = (CR[39] & (-CR[39]));
																																															Rows[4] |= LS[39];
																																															Columns[3] |= LS[39];
																																															OC[LS[39]] |= LS[31];
																																															OC[LS[31]] |= LS[39];
																																															for (CR[33] = AllN ^ (Rows[3] | Columns[6]); ((CR[33] != 0)); CR[33] = (CR[33] & (CR[33] - 1))) {
																																																LS[33] = (CR[33] & (-CR[33]));
																																																Rows[3] |= LS[33];
																																																Columns[6] |= LS[33];
																																																for (CR[57] = AllN ^ (Rows[6] | Columns[3] | OC[LS[33]]); ((CR[57] != 0)); CR[57] = (CR[57] & (CR[57] - 1))) {
																																																	LS[57] = (CR[57] & (-CR[57]));
																																																	Rows[6] |= LS[57];
																																																	Columns[3] |= LS[57];
																																																	OC[LS[57]] |= LS[33];
																																																	OC[LS[33]] |= LS[57];
																																																	for (CR[34] = AllN ^ (Rows[3] | Columns[7]); ((CR[34] != 0)); CR[34] = (CR[34] & (CR[34] - 1))) {
																																																		LS[34] = (CR[34] & (-CR[34]));
																																																		Rows[3] |= LS[34];
																																																		Columns[7] |= LS[34];
																																																		for (CR[66] = AllN ^ (Rows[7] | Columns[3] | OC[LS[34]]); ((CR[66] != 0)); CR[66] = (CR[66] & (CR[66] - 1))) {
																																																			LS[66] = (CR[66] & (-CR[66]));
																																																			Rows[7] |= LS[66];
																																																			Columns[3] |= LS[66];
																																																			OC[LS[66]] |= LS[34];
																																																			OC[LS[34]] |= LS[66];
																																																			CR[35] = AllN ^ (Rows[3] | Columns[8]);
																																																			if ((CR[35] != 0)) {
																																																				LS[35] = (CR[35] & (-CR[35]));
																																																				Columns[8] |= LS[35];
																																																				CR[75] = AllN ^ (Rows[8] | Columns[3] | OC[LS[35]]);
																																																				if ((CR[75] != 0)) {
																																																					LS[75] = (CR[75] & (-CR[75]));
																																																					Rows[8] |= LS[75];
																																																					OC[LS[75]] |= LS[35];
																																																					OC[LS[35]] |= LS[75];
																																																					for (CR[41] = AllN ^ (Rows[4] | Columns[5]); ((CR[41] != 0)); CR[41] = (CR[41] & (CR[41] - 1))) {
																																																						LS[41] = (CR[41] & (-CR[41]));
																																																						Rows[4] |= LS[41];
																																																						Columns[5] |= LS[41];
																																																						for (CR[49] = AllN ^ (Rows[5] | Columns[4] | OC[LS[41]]); ((CR[49] != 0)); CR[49] = (CR[49] & (CR[49] - 1))) {
																																																							LS[49] = (CR[49] & (-CR[49]));
																																																							Rows[5] |= LS[49];
																																																							Columns[4] |= LS[49];
																																																							OC[LS[49]] |= LS[41];
																																																							OC[LS[41]] |= LS[49];
																																																							for (CR[42] = AllN ^ (Rows[4] | Columns[6]); ((CR[42] != 0)); CR[42] = (CR[42] & (CR[42] - 1))) {
																																																								LS[42] = (CR[42] & (-CR[42]));
																																																								Rows[4] |= LS[42];
																																																								Columns[6] |= LS[42];
																																																								for (CR[58] = AllN ^ (Rows[6] | Columns[4] | OC[LS[42]]); ((CR[58] != 0)); CR[58] = (CR[58] & (CR[58] - 1))) {
																																																									LS[58] = (CR[58] & (-CR[58]));
																																																									Rows[6] |= LS[58];
																																																									Columns[4] |= LS[58];
																																																									OC[LS[58]] |= LS[42];
																																																									OC[LS[42]] |= LS[58];
																																																									for (CR[43] = AllN ^ (Rows[4] | Columns[7]); ((CR[43] != 0)); CR[43] = (CR[43] & (CR[43] - 1))) {
																																																										LS[43] = (CR[43] & (-CR[43]));
																																																										Rows[4] |= LS[43];
																																																										Columns[7] |= LS[43];
																																																										for (CR[67] = AllN ^ (Rows[7] | Columns[4] | OC[LS[43]]); ((CR[67] != 0)); CR[67] = (CR[67] & (CR[67] - 1))) {
																																																											LS[67] = (CR[67] & (-CR[67]));
																																																											Rows[7] |= LS[67];
																																																											Columns[4] |= LS[67];
																																																											OC[LS[67]] |= LS[43];
																																																											OC[LS[43]] |= LS[67];
																																																											CR[44] = AllN ^ (Rows[4] | Columns[8]);
																																																											if ((CR[44] != 0)) {
																																																												LS[44] = (CR[44] & (-CR[44]));
																																																												Columns[8] |= LS[44];
																																																												CR[76] = AllN ^ (Rows[8] | Columns[4] | OC[LS[44]]);
																																																												if ((CR[76] != 0)) {
																																																													LS[76] = (CR[76] & (-CR[76]));
																																																													Rows[8] |= LS[76];
																																																													OC[LS[76]] |= LS[44];
																																																													OC[LS[44]] |= LS[76];
																																																													for (CR[51] = AllN ^ (Rows[5] | Columns[6]); ((CR[51] != 0)); CR[51] = (CR[51] & (CR[51] - 1))) {
																																																														LS[51] = (CR[51] & (-CR[51]));
																																																														Rows[5] |= LS[51];
																																																														Columns[6] |= LS[51];
																																																														for (CR[59] = AllN ^ (Rows[6] | Columns[5] | OC[LS[51]]); ((CR[59] != 0)); CR[59] = (CR[59] & (CR[59] - 1))) {
																																																															LS[59] = (CR[59] & (-CR[59]));
																																																															Rows[6] |= LS[59];
																																																															Columns[5] |= LS[59];
																																																															OC[LS[59]] |= LS[51];
																																																															OC[LS[51]] |= LS[59];
																																																															for (CR[52] = AllN ^ (Rows[5] | Columns[7]); ((CR[52] != 0)); CR[52] = (CR[52] & (CR[52] - 1))) {
																																																																LS[52] = (CR[52] & (-CR[52]));
																																																																Rows[5] |= LS[52];
																																																																Columns[7] |= LS[52];
																																																																for (CR[68] = AllN ^ (Rows[7] | Columns[5] | OC[LS[52]]); ((CR[68] != 0)); CR[68] = (CR[68] & (CR[68] - 1))) {
																																																																	LS[68] = (CR[68] & (-CR[68]));
																																																																	Rows[7] |= LS[68];
																																																																	Columns[5] |= LS[68];
																																																																	OC[LS[68]] |= LS[52];
																																																																	OC[LS[52]] |= LS[68];
																																																																	CR[53] = AllN ^ (Rows[5] | Columns[8]);
																																																																	if ((CR[53] != 0)) {
																																																																		LS[53] = (CR[53] & (-CR[53]));
																																																																		Columns[8] |= LS[53];
																																																																		CR[77] = AllN ^ (Rows[8] | Columns[5] | OC[LS[53]]);
																																																																		if ((CR[77] != 0)) {
																																																																			LS[77] = (CR[77] & (-CR[77]));
																																																																			Rows[8] |= LS[77];
																																																																			OC[LS[77]] |= LS[53];
																																																																			OC[LS[53]] |= LS[77];
																																																																			for (CR[61] = AllN ^ (Rows[6] | Columns[7]); ((CR[61] != 0)); CR[61] = (CR[61] & (CR[61] - 1))) {
																																																																				LS[61] = (CR[61] & (-CR[61]));
																																																																				Rows[6] |= LS[61];
																																																																				Columns[7] |= LS[61];
																																																																				for (CR[69] = AllN ^ (Rows[7] | Columns[6] | OC[LS[61]]); ((CR[69] != 0)); CR[69] = (CR[69] & (CR[69] - 1))) {
																																																																					LS[69] = (CR[69] & (-CR[69]));
																																																																					Rows[7] |= LS[69];
																																																																					Columns[6] |= LS[69];
																																																																					OC[LS[69]] |= LS[61];
																																																																					OC[LS[61]] |= LS[69];
																																																																					CR[62] = AllN ^ (Rows[6] | Columns[8]);
																																																																					if ((CR[62] != 0)) {
																																																																						LS[62] = (CR[62] & (-CR[62]));
																																																																						Columns[8] |= LS[62];
																																																																						CR[78] = AllN ^ (Rows[8] | Columns[6] | OC[LS[62]]);
																																																																						if ((CR[78] != 0)) {
																																																																							LS[78] = (CR[78] & (-CR[78]));
																																																																							Rows[8] |= LS[78];
																																																																							OC[LS[78]] |= LS[62];
																																																																							OC[LS[62]] |= LS[78];
																																																																							CR[71] = AllN ^ (Rows[7] | Columns[8]);
																																																																							if ((CR[71] != 0)) {
																																																																								LS[71] = (CR[71] & (-CR[71]));
																																																																								CR[79] = AllN ^ (Rows[8] | Columns[7] | OC[LS[71]]);
																																																																								if ((CR[79] != 0)) {
																																																																									LS[79] = (CR[79] & (-CR[79]));
																																																																									OC[LS[79]] |= LS[71];
																																																																									OC[LS[71]] |= LS[79];
																																																																									#pragma omp critical
																																																																									{
																																																																										is_SODLS(LS);
																																																																										pr_cnt++;
																																																																									}
																																																																									/*if (SquaresCnt % 10 == 0) {
																																																																										StopProf();
																																																																										cout << "Squares found: " << SquaresCnt << endl;
																																																																										cout << "Average time: " << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << " ms per square" << endl;
																																																																										cout << "Squares per second: " << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;
																																																																									}*/
																																																																									OC[LS[79]] ^= LS[71];
																																																																									OC[LS[71]] ^= LS[79];
																																																																								}
																																																																							}
																																																																							Rows[8] ^= LS[78];
																																																																							OC[LS[78]] ^= LS[62];
																																																																							OC[LS[62]] ^= LS[78];
																																																																						}
																																																																						Columns[8] ^= LS[62];
																																																																					}
																																																																					Rows[7] ^= LS[69];
																																																																					Columns[6] ^= LS[69];
																																																																					OC[LS[69]] ^= LS[61];
																																																																					OC[LS[61]] ^= LS[69];
																																																																				}
																																																																				Rows[6] ^= LS[61];
																																																																				Columns[7] ^= LS[61];
																																																																			}
																																																																			Rows[8] ^= LS[77];
																																																																			OC[LS[77]] ^= LS[53];
																																																																			OC[LS[53]] ^= LS[77];
																																																																		}
																																																																		Columns[8] ^= LS[53];
																																																																	}
																																																																	Rows[7] ^= LS[68];
																																																																	Columns[5] ^= LS[68];
																																																																	OC[LS[68]] ^= LS[52];
																																																																	OC[LS[52]] ^= LS[68];
																																																																}
																																																																Rows[5] ^= LS[52];
																																																																Columns[7] ^= LS[52];
																																																															}
																																																															Rows[6] ^= LS[59];
																																																															Columns[5] ^= LS[59];
																																																															OC[LS[59]] ^= LS[51];
																																																															OC[LS[51]] ^= LS[59];
																																																														}
																																																														Rows[5] ^= LS[51];
																																																														Columns[6] ^= LS[51];
																																																													}
																																																													Rows[8] ^= LS[76];
																																																													OC[LS[76]] ^= LS[44];
																																																													OC[LS[44]] ^= LS[76];
																																																												}
																																																												Columns[8] ^= LS[44];
																																																											}
																																																											Rows[7] ^= LS[67];
																																																											Columns[4] ^= LS[67];
																																																											OC[LS[67]] ^= LS[43];
																																																											OC[LS[43]] ^= LS[67];
																																																										}
																																																										Rows[4] ^= LS[43];
																																																										Columns[7] ^= LS[43];
																																																									}
																																																									Rows[6] ^= LS[58];
																																																									Columns[4] ^= LS[58];
																																																									OC[LS[58]] ^= LS[42];
																																																									OC[LS[42]] ^= LS[58];
																																																								}
																																																								Rows[4] ^= LS[42];
																																																								Columns[6] ^= LS[42];
																																																							}
																																																							Rows[5] ^= LS[49];
																																																							Columns[4] ^= LS[49];
																																																							OC[LS[49]] ^= LS[41];
																																																							OC[LS[41]] ^= LS[49];
																																																						}
																																																						Rows[4] ^= LS[41];
																																																						Columns[5] ^= LS[41];
																																																					}
																																																					Rows[8] ^= LS[75];
																																																					OC[LS[75]] ^= LS[35];
																																																					OC[LS[35]] ^= LS[75];
																																																				}
																																																				Columns[8] ^= LS[35];
																																																			}
																																																			Rows[7] ^= LS[66];
																																																			Columns[3] ^= LS[66];
																																																			OC[LS[66]] ^= LS[34];
																																																			OC[LS[34]] ^= LS[66];
																																																		}
																																																		Rows[3] ^= LS[34];
																																																		Columns[7] ^= LS[34];
																																																	}
																																																	Rows[6] ^= LS[57];
																																																	Columns[3] ^= LS[57];
																																																	OC[LS[57]] ^= LS[33];
																																																	OC[LS[33]] ^= LS[57];
																																																}
																																																Rows[3] ^= LS[33];
																																																Columns[6] ^= LS[33];
																																															}
																																															Rows[4] ^= LS[39];
																																															Columns[3] ^= LS[39];
																																															OC[LS[39]] ^= LS[31];
																																															OC[LS[31]] ^= LS[39];
																																														}
																																														Rows[3] ^= LS[31];
																																														Columns[4] ^= LS[31];
																																													}
																																													Rows[8] ^= LS[74];
																																													OC[LS[74]] ^= LS[26];
																																													OC[LS[26]] ^= LS[74];
																																												}
																																												Columns[8] ^= LS[26];
																																											}
																																											Rows[7] ^= LS[65];
																																											Columns[2] ^= LS[65];
																																											OC[LS[65]] ^= LS[25];
																																											OC[LS[25]] ^= LS[65];
																																										}
																																										Rows[2] ^= LS[25];
																																										Columns[7] ^= LS[25];
																																									}
																																									Rows[4] ^= LS[38];
																																									Columns[2] ^= LS[38];
																																									OC[LS[38]] ^= LS[22];
																																									OC[LS[22]] ^= LS[38];
																																								}
																																								Rows[2] ^= LS[22];
																																								Columns[4] ^= LS[22];
																																							}
																																							Rows[5] ^= LS[47];
																																							Columns[2] ^= LS[47];
																																							OC[LS[47]] ^= LS[23];
																																							OC[LS[23]] ^= LS[47];
																																						}
																																						Rows[2] ^= LS[23];
																																						Columns[5] ^= LS[23];
																																					}
																																					Rows[3] ^= LS[29];
																																					Columns[2] ^= LS[29];
																																					OC[LS[29]] ^= LS[21];
																																					OC[LS[21]] ^= LS[29];
																																				}
																																				Rows[2] ^= LS[21];
																																				Columns[3] ^= LS[21];
																																			}
																																			Rows[8] ^= LS[73];
																																			OC[LS[73]] ^= LS[17];
																																			OC[LS[17]] ^= LS[73];
																																		}
																																		Columns[8] ^= LS[17];
																																	}
																																	Rows[4] ^= LS[37];
																																	Columns[1] ^= LS[37];
																																	OC[LS[37]] ^= LS[13];
																																	OC[LS[13]] ^= LS[37];
																																}
																																Rows[1] ^= LS[13];
																																Columns[4] ^= LS[13];
																															}
																															Rows[6] ^= LS[55];
																															Columns[1] ^= LS[55];
																															OC[LS[55]] ^= LS[15];
																															OC[LS[15]] ^= LS[55];
																														}
																														Rows[1] ^= LS[15];
																														Columns[6] ^= LS[15];
																													}
																													Rows[5] ^= LS[46];
																													Columns[1] ^= LS[46];
																													OC[LS[46]] ^= LS[14];
																													OC[LS[14]] ^= LS[46];
																												}
																												Rows[1] ^= LS[14];
																												Columns[5] ^= LS[14];
																											}
																											Rows[3] ^= LS[28];
																											Columns[1] ^= LS[28];
																											OC[LS[28]] ^= LS[12];
																											OC[LS[12]] ^= LS[28];
																										}
																										Rows[1] ^= LS[12];
																										Columns[3] ^= LS[12];
																									}
																									Rows[2] ^= LS[19];
																									Columns[1] ^= LS[19];
																									OC[LS[19]] ^= LS[11];
																									OC[LS[11]] ^= LS[19];
																								}
																								Rows[1] ^= LS[11];
																								Columns[2] ^= LS[11];
																							}
																							Rows[5] ^= LS[48];
																							Columns[3] ^= LS[48];
																							OC[LS[48]] ^= LS[32];
																							OC[LS[32]] ^= LS[48];
																						}
																						Rows[3] ^= LS[32];
																						Columns[5] ^= LS[32];
																						AD ^= LS[32];
																					}
																					Rows[6] ^= LS[56];
																					Columns[2] ^= LS[56];
																					AD ^= LS[56];
																					OC[LS[56]] ^= LS[24];
																					OC[LS[24]] ^= LS[56];
																				}
																				Rows[2] ^= LS[24];
																				Columns[6] ^= LS[24];
																				AD ^= LS[24];
																			}
																			Rows[7] ^= LS[64];
																			Columns[1] ^= LS[64];
																			AD ^= LS[64];
																			OC[LS[64]] ^= LS[16];
																			OC[LS[16]] ^= LS[64];
																		}
																		Rows[1] ^= LS[16];
																		Columns[7] ^= LS[16];
																		AD ^= LS[16];
																	}
																	Rows[8] ^= LS[72];
																	AD ^= LS[72];
																	OC[LS[72]] ^= LS[8];
																	OC[LS[8]] ^= LS[72];
																}
																Rows[7] ^= LS[63];
																Columns[0] ^= LS[63];
																OC[LS[63]] ^= LS[7];
																OC[LS[7]] ^= LS[63];
															}
															Rows[6] ^= LS[54];
															Columns[0] ^= LS[54];
															OC[LS[54]] ^= LS[6];
															OC[LS[6]] ^= LS[54];
														}
														Rows[5] ^= LS[45];
														Columns[0] ^= LS[45];
														OC[LS[45]] ^= LS[5];
														OC[LS[5]] ^= LS[45];
													}
													Rows[4] ^= LS[36];
													Columns[0] ^= LS[36];
													OC[LS[36]] ^= LS[4];
													OC[LS[4]] ^= LS[36];
												}
												Rows[3] ^= LS[27];
												Columns[0] ^= LS[27];
												OC[LS[27]] ^= LS[3];
												OC[LS[3]] ^= LS[27];
											}
											Rows[2] ^= LS[18];
											Columns[0] ^= LS[18];
											OC[LS[18]] ^= LS[2];
											OC[LS[2]] ^= LS[18];
										}
										Rows[1] ^= LS[9];
										Columns[0] ^= LS[9];
										OC[LS[9]] ^= LS[1];
										OC[LS[1]] ^= LS[9];
									}

									Rows[8] ^= LS[80];
									Columns[8] ^= LS[80];
									OC[LS[80]] ^= LS[80];
								}
								Rows[7] ^= LS[70];
								Columns[7] ^= LS[70];
								MD ^= LS[70];
								OC[LS[70]] ^= LS[70];
							}
							Rows[6] ^= LS[60];
							Columns[6] ^= LS[60];
							MD ^= LS[60];
							OC[LS[60]] ^= LS[60];
						}
						Rows[5] ^= LS[50];
						Columns[5] ^= LS[50];
						MD ^= LS[50];
						OC[LS[50]] ^= LS[50];
					}
					Rows[4] ^= LS[40];
					Columns[4] ^= LS[40];
					MD ^= LS[40];
					AD ^= LS[40];
					OC[LS[40]] ^= LS[40];
				}
				Rows[3] ^= LS[30];
				Columns[3] ^= LS[30];
				MD ^= LS[30];
				OC[LS[30]] ^= LS[30];
			}
			Rows[2] ^= LS[20];
			Columns[2] ^= LS[20];
			MD ^= LS[20];
			OC[LS[20]] ^= LS[20];
		}
		Rows[1] ^= LS[10];
		Columns[1] ^= LS[10];
		MD ^= LS[10];
		OC[LS[10]] ^= LS[10];
	}
	cur_cnt++;
	return pr_cnt;
}
/*
void Generate(){
	StartProf();
	for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); ((CR[10] != 0)); CR[10] = (CR[10] & (CR[10] - 1))) {
		LS[10] = (CR[10] & (-CR[10]));
		Rows[1] |= LS[10];
		Columns[1] |= LS[10];
		MD |= LS[10];
		OC[LS[10]] |= LS[10];
		for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); ((CR[20] != 0)); CR[20] = (CR[20] & (CR[20] - 1))) {
			LS[20] = (CR[20] & (-CR[20]));
			Rows[2] |= LS[20];
			Columns[2] |= LS[20];
			MD |= LS[20];
			OC[LS[20]] |= LS[20];
			for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); ((CR[30] != 0)); CR[30] = (CR[30] & (CR[30] - 1))) {
				LS[30] = (CR[30] & (-CR[30]));
				Rows[3] |= LS[30];
				Columns[3] |= LS[30];
				MD |= LS[30];
				OC[LS[30]] |= LS[30];
				for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); ((CR[40] != 0)); CR[40] = (CR[40] & (CR[40] - 1))) {
					LS[40] = (CR[40] & (-CR[40]));
					Rows[4] |= LS[40];
					Columns[4] |= LS[40];
					MD |= LS[40];
					AD |= LS[40];
					OC[LS[40]] |= LS[40];
					for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); ((CR[50] != 0)); CR[50] = (CR[50] & (CR[50] - 1))) {
						LS[50] = (CR[50] & (-CR[50]));
						Rows[5] |= LS[50];
						Columns[5] |= LS[50];
						MD |= LS[50];
						OC[LS[50]] |= LS[50];
						for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); ((CR[60] != 0)); CR[60] = (CR[60] & (CR[60] - 1))) {
							LS[60] = (CR[60] & (-CR[60]));
							Rows[6] |= LS[60];
							Columns[6] |= LS[60];
							MD |= LS[60];
							OC[LS[60]] |= LS[60];
							for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); ((CR[70] != 0)); CR[70] = (CR[70] & (CR[70] - 1))) {
								LS[70] = (CR[70] & (-CR[70]));
								Rows[7] |= LS[70];
								Columns[7] |= LS[70];
								MD |= LS[70];
								OC[LS[70]] |= LS[70];
								CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
								if ((CR[80] != 0)) {
									LS[80] = (CR[80] & (-CR[80]));
									Rows[8] |= LS[80];
									Columns[8] |= LS[80];
									OC[LS[80]] |= LS[80];
								
									for (CR[9] = AllN ^ (Rows[1] | Columns[0] | OC[LS[1]]); ((CR[9] != 0)); CR[9] = (CR[9] & (CR[9] - 1))) {
										LS[9] = (CR[9] & (-CR[9]));
										Rows[1] |= LS[9];
										Columns[0] |= LS[9];
										OC[LS[9]] |= LS[1];
										OC[LS[1]] |= LS[9];
										for (CR[18] = AllN ^ (Rows[2] | Columns[0] | OC[LS[2]]); ((CR[18] != 0)); CR[18] = (CR[18] & (CR[18] - 1))) {
											LS[18] = (CR[18] & (-CR[18]));
											Rows[2] |= LS[18];
											Columns[0] |= LS[18];
											OC[LS[18]] |= LS[2];
											OC[LS[2]] |= LS[18];
											for (CR[27] = AllN ^ (Rows[3] | Columns[0] | OC[LS[3]]); ((CR[27] != 0)); CR[27] = (CR[27] & (CR[27] - 1))) {
												LS[27] = (CR[27] & (-CR[27]));
												Rows[3] |= LS[27];
												Columns[0] |= LS[27];
												OC[LS[27]] |= LS[3];
												OC[LS[3]] |= LS[27];
												for (CR[36] = AllN ^ (Rows[4] | Columns[0] | OC[LS[4]]); ((CR[36] != 0)); CR[36] = (CR[36] & (CR[36] - 1))) {
													LS[36] = (CR[36] & (-CR[36]));
													Rows[4] |= LS[36];
													Columns[0] |= LS[36];
													OC[LS[36]] |= LS[4];
													OC[LS[4]] |= LS[36];
													for (CR[45] = AllN ^ (Rows[5] | Columns[0] | OC[LS[5]]); ((CR[45] != 0)); CR[45] = (CR[45] & (CR[45] - 1))) {
														LS[45] = (CR[45] & (-CR[45]));
														Rows[5] |= LS[45];
														Columns[0] |= LS[45];
														OC[LS[45]] |= LS[5];
														OC[LS[5]] |= LS[45];
														for (CR[54] = AllN ^ (Rows[6] | Columns[0] | OC[LS[6]]); ((CR[54] != 0)); CR[54] = (CR[54] & (CR[54] - 1))) {
															LS[54] = (CR[54] & (-CR[54]));
															Rows[6] |= LS[54];
															Columns[0] |= LS[54];
															OC[LS[54]] |= LS[6];
															OC[LS[6]] |= LS[54];
															for (CR[63] = AllN ^ (Rows[7] | Columns[0] | OC[LS[7]]); ((CR[63] != 0)); CR[63] = (CR[63] & (CR[63] - 1))) {
																LS[63] = (CR[63] & (-CR[63]));
																Rows[7] |= LS[63];
																Columns[0] |= LS[63];
																OC[LS[63]] |= LS[7];
																OC[LS[7]] |= LS[63];
																CR[72] = AllN ^ (Rows[8] | Columns[0] | OC[LS[8]] | AD);
																if ((CR[72] != 0)) {
																	LS[72] = (CR[72] & (-CR[72]));
																	Rows[8] |= LS[72];
																	AD |= LS[72];
																	OC[LS[72]] |= LS[8];
																	OC[LS[8]] |= LS[72];
																	for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); ((CR[16] != 0)); CR[16] = (CR[16] & (CR[16] - 1))) {
																		LS[16] = (CR[16] & (-CR[16]));
																		Rows[1] |= LS[16];
																		Columns[7] |= LS[16];
																		AD |= LS[16];
																		for (CR[64] = AllN ^ (Rows[7] | Columns[1] | OC[LS[16]] | AD); ((CR[64] != 0)); CR[64] = (CR[64] & (CR[64] - 1))) {
																			LS[64] = (CR[64] & (-CR[64]));
																			Rows[7] |= LS[64];
																			Columns[1] |= LS[64];
																			AD |= LS[64];
																			OC[LS[64]] |= LS[16];
																			OC[LS[16]] |= LS[64];
																			for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); ((CR[24] != 0)); CR[24] = (CR[24] & (CR[24] - 1))) {
																				LS[24] = (CR[24] & (-CR[24]));
																				Rows[2] |= LS[24];
																				Columns[6] |= LS[24];
																				AD |= LS[24];
																				for (CR[56] = AllN ^ (Rows[6] | Columns[2] | OC[LS[24]] | AD); ((CR[56] != 0)); CR[56] = (CR[56] & (CR[56] - 1))) {
																					LS[56] = (CR[56] & (-CR[56]));
																					Rows[6] |= LS[56];
																					Columns[2] |= LS[56];
																					AD |= LS[56];
																					OC[LS[56]] |= LS[24];
																					OC[LS[24]] |= LS[56];
																					for (CR[32] = AllN ^ (Rows[3] | Columns[5] | AD); ((CR[32] != 0)); CR[32] = (CR[32] & (CR[32] - 1))) {
																						LS[32] = (CR[32] & (-CR[32]));
																						Rows[3] |= LS[32];
																						Columns[5] |= LS[32];
																						AD |= LS[32];
																						CR[48] = AllN ^ (Rows[5] | Columns[3] | OC[LS[32]] | AD);
																						if ((CR[48] != 0)) {
																							LS[48] = (CR[48] & (-CR[48]));
																							Rows[5] |= LS[48];
																							Columns[3] |= LS[48];
																							OC[LS[48]] |= LS[32];
																							OC[LS[32]] |= LS[48];
																							for (CR[11] = AllN ^ (Rows[1] | Columns[2]); ((CR[11] != 0)); CR[11] = (CR[11] & (CR[11] - 1))) {
																								LS[11] = (CR[11] & (-CR[11]));
																								Rows[1] |= LS[11];
																								Columns[2] |= LS[11];
																								for (CR[19] = AllN ^ (Rows[2] | Columns[1] | OC[LS[11]]); ((CR[19] != 0)); CR[19] = (CR[19] & (CR[19] - 1))) {
																									LS[19] = (CR[19] & (-CR[19]));
																									Rows[2] |= LS[19];
																									Columns[1] |= LS[19];
																									OC[LS[19]] |= LS[11];
																									OC[LS[11]] |= LS[19];
																									for (CR[12] = AllN ^ (Rows[1] | Columns[3]); ((CR[12] != 0)); CR[12] = (CR[12] & (CR[12] - 1))) {
																										LS[12] = (CR[12] & (-CR[12]));
																										Rows[1] |= LS[12];
																										Columns[3] |= LS[12];
																										for (CR[28] = AllN ^ (Rows[3] | Columns[1] | OC[LS[12]]); ((CR[28] != 0)); CR[28] = (CR[28] & (CR[28] - 1))) {
																											LS[28] = (CR[28] & (-CR[28]));
																											Rows[3] |= LS[28];
																											Columns[1] |= LS[28];
																											OC[LS[28]] |= LS[12];
																											OC[LS[12]] |= LS[28];
																											for (CR[14] = AllN ^ (Rows[1] | Columns[5]); ((CR[14] != 0)); CR[14] = (CR[14] & (CR[14] - 1))) {
																												LS[14] = (CR[14] & (-CR[14]));
																												Rows[1] |= LS[14];
																												Columns[5] |= LS[14];
																												for (CR[46] = AllN ^ (Rows[5] | Columns[1] | OC[LS[14]]); ((CR[46] != 0)); CR[46] = (CR[46] & (CR[46] - 1))) {
																													LS[46] = (CR[46] & (-CR[46]));
																													Rows[5] |= LS[46];
																													Columns[1] |= LS[46];
																													OC[LS[46]] |= LS[14];
																													OC[LS[14]] |= LS[46];
																													for (CR[15] = AllN ^ (Rows[1] | Columns[6]); ((CR[15] != 0)); CR[15] = (CR[15] & (CR[15] - 1))) {
																														LS[15] = (CR[15] & (-CR[15]));
																														Rows[1] |= LS[15];
																														Columns[6] |= LS[15];
																														for (CR[55] = AllN ^ (Rows[6] | Columns[1] | OC[LS[15]]); ((CR[55] != 0)); CR[55] = (CR[55] & (CR[55] - 1))) {
																															LS[55] = (CR[55] & (-CR[55]));
																															Rows[6] |= LS[55];
																															Columns[1] |= LS[55];
																															OC[LS[55]] |= LS[15];
																															OC[LS[15]] |= LS[55];
																															for (CR[13] = AllN ^ (Rows[1] | Columns[4]); ((CR[13] != 0)); CR[13] = (CR[13] & (CR[13] - 1))) {
																																LS[13] = (CR[13] & (-CR[13]));
																																Rows[1] |= LS[13];
																																Columns[4] |= LS[13];
																																for (CR[37] = AllN ^ (Rows[4] | Columns[1] | OC[LS[13]]); ((CR[37] != 0)); CR[37] = (CR[37] & (CR[37] - 1))) {
																																	LS[37] = (CR[37] & (-CR[37]));
																																	Rows[4] |= LS[37];
																																	Columns[1] |= LS[37];
																																	OC[LS[37]] |= LS[13];
																																	OC[LS[13]] |= LS[37];
																																	CR[17] = AllN ^ (Rows[1] | Columns[8]);
																																	if ((CR[17] != 0)) {
																																		LS[17] = (CR[17] & (-CR[17]));
																																		Columns[8] |= LS[17];
																																		CR[73] = AllN ^ (Rows[8] | Columns[1] | OC[LS[17]]);
																																		if ((CR[73] != 0)) {
																																			LS[73] = (CR[73] & (-CR[73]));
																																			Rows[8] |= LS[73];
																																			OC[LS[73]] |= LS[17];
																																			OC[LS[17]] |= LS[73];
																																			for (CR[21] = AllN ^ (Rows[2] | Columns[3]); ((CR[21] != 0)); CR[21] = (CR[21] & (CR[21] - 1))) {
																																				LS[21] = (CR[21] & (-CR[21]));
																																				Rows[2] |= LS[21];
																																				Columns[3] |= LS[21];
																																				for (CR[29] = AllN ^ (Rows[3] | Columns[2] | OC[LS[21]]); ((CR[29] != 0)); CR[29] = (CR[29] & (CR[29] - 1))) {
																																					LS[29] = (CR[29] & (-CR[29]));
																																					Rows[3] |= LS[29];
																																					Columns[2] |= LS[29];
																																					OC[LS[29]] |= LS[21];
																																					OC[LS[21]] |= LS[29];
																																					for (CR[23] = AllN ^ (Rows[2] | Columns[5]); ((CR[23] != 0)); CR[23] = (CR[23] & (CR[23] - 1))) {
																																						LS[23] = (CR[23] & (-CR[23]));
																																						Rows[2] |= LS[23];
																																						Columns[5] |= LS[23];
																																						for (CR[47] = AllN ^ (Rows[5] | Columns[2] | OC[LS[23]]); ((CR[47] != 0)); CR[47] = (CR[47] & (CR[47] - 1))) {
																																							LS[47] = (CR[47] & (-CR[47]));
																																							Rows[5] |= LS[47];
																																							Columns[2] |= LS[47];
																																							OC[LS[47]] |= LS[23];
																																							OC[LS[23]] |= LS[47];
																																							for (CR[22] = AllN ^ (Rows[2] | Columns[4]); ((CR[22] != 0)); CR[22] = (CR[22] & (CR[22] - 1))) {
																																								LS[22] = (CR[22] & (-CR[22]));
																																								Rows[2] |= LS[22];
																																								Columns[4] |= LS[22];
																																								for (CR[38] = AllN ^ (Rows[4] | Columns[2] | OC[LS[22]]); ((CR[38] != 0)); CR[38] = (CR[38] & (CR[38] - 1))) {
																																									LS[38] = (CR[38] & (-CR[38]));
																																									Rows[4] |= LS[38];
																																									Columns[2] |= LS[38];
																																									OC[LS[38]] |= LS[22];
																																									OC[LS[22]] |= LS[38];
																																									for (CR[25] = AllN ^ (Rows[2] | Columns[7]); ((CR[25] != 0)); CR[25] = (CR[25] & (CR[25] - 1))) {
																																										LS[25] = (CR[25] & (-CR[25]));
																																										Rows[2] |= LS[25];
																																										Columns[7] |= LS[25];
																																										for (CR[65] = AllN ^ (Rows[7] | Columns[2] | OC[LS[25]]); ((CR[65] != 0)); CR[65] = (CR[65] & (CR[65] - 1))) {
																																											LS[65] = (CR[65] & (-CR[65]));
																																											Rows[7] |= LS[65];
																																											Columns[2] |= LS[65];
																																											OC[LS[65]] |= LS[25];
																																											OC[LS[25]] |= LS[65];
																																											CR[26] = AllN ^ (Rows[2] | Columns[8]);
																																											if ((CR[26] != 0)) {
																																												LS[26] = (CR[26] & (-CR[26]));
																																												Columns[8] |= LS[26];
																																												CR[74] = AllN ^ (Rows[8] | Columns[2] | OC[LS[26]]);
																																												if ((CR[74] != 0)) {
																																													LS[74] = (CR[74] & (-CR[74]));
																																													Rows[8] |= LS[74];
																																													OC[LS[74]] |= LS[26];
																																													OC[LS[26]] |= LS[74];
																																													for (CR[31] = AllN ^ (Rows[3] | Columns[4]); ((CR[31] != 0)); CR[31] = (CR[31] & (CR[31] - 1))) {
																																														LS[31] = (CR[31] & (-CR[31]));
																																														Rows[3] |= LS[31];
																																														Columns[4] |= LS[31];
																																														for (CR[39] = AllN ^ (Rows[4] | Columns[3] | OC[LS[31]]); ((CR[39] != 0)); CR[39] = (CR[39] & (CR[39] - 1))) {
																																															LS[39] = (CR[39] & (-CR[39]));
																																															Rows[4] |= LS[39];
																																															Columns[3] |= LS[39];
																																															OC[LS[39]] |= LS[31];
																																															OC[LS[31]] |= LS[39];
																																															for (CR[33] = AllN ^ (Rows[3] | Columns[6]); ((CR[33] != 0)); CR[33] = (CR[33] & (CR[33] - 1))) {
																																																LS[33] = (CR[33] & (-CR[33]));
																																																Rows[3] |= LS[33];
																																																Columns[6] |= LS[33];
																																																for (CR[57] = AllN ^ (Rows[6] | Columns[3] | OC[LS[33]]); ((CR[57] != 0)); CR[57] = (CR[57] & (CR[57] - 1))) {
																																																	LS[57] = (CR[57] & (-CR[57]));
																																																	Rows[6] |= LS[57];
																																																	Columns[3] |= LS[57];
																																																	OC[LS[57]] |= LS[33];
																																																	OC[LS[33]] |= LS[57];
																																																	for (CR[34] = AllN ^ (Rows[3] | Columns[7]); ((CR[34] != 0)); CR[34] = (CR[34] & (CR[34] - 1))) {
																																																		LS[34] = (CR[34] & (-CR[34]));
																																																		Rows[3] |= LS[34];
																																																		Columns[7] |= LS[34];
																																																		for (CR[66] = AllN ^ (Rows[7] | Columns[3] | OC[LS[34]]); ((CR[66] != 0)); CR[66] = (CR[66] & (CR[66] - 1))) {
																																																			LS[66] = (CR[66] & (-CR[66]));
																																																			Rows[7] |= LS[66];
																																																			Columns[3] |= LS[66];
																																																			OC[LS[66]] |= LS[34];
																																																			OC[LS[34]] |= LS[66];
																																																			CR[35] = AllN ^ (Rows[3] | Columns[8]);
																																																			if ((CR[35] != 0)) {
																																																				LS[35] = (CR[35] & (-CR[35]));
																																																				Columns[8] |= LS[35];
																																																				CR[75] = AllN ^ (Rows[8] | Columns[3] | OC[LS[35]]);
																																																				if ((CR[75] != 0)) {
																																																					LS[75] = (CR[75] & (-CR[75]));
																																																					Rows[8] |= LS[75];
																																																					OC[LS[75]] |= LS[35];
																																																					OC[LS[35]] |= LS[75];
																																																					for (CR[41] = AllN ^ (Rows[4] | Columns[5]); ((CR[41] != 0)); CR[41] = (CR[41] & (CR[41] - 1))) {
																																																						LS[41] = (CR[41] & (-CR[41]));
																																																						Rows[4] |= LS[41];
																																																						Columns[5] |= LS[41];
																																																						for (CR[49] = AllN ^ (Rows[5] | Columns[4] | OC[LS[41]]); ((CR[49] != 0)); CR[49] = (CR[49] & (CR[49] - 1))) {
																																																							LS[49] = (CR[49] & (-CR[49]));
																																																							Rows[5] |= LS[49];
																																																							Columns[4] |= LS[49];
																																																							OC[LS[49]] |= LS[41];
																																																							OC[LS[41]] |= LS[49];
																																																							for (CR[42] = AllN ^ (Rows[4] | Columns[6]); ((CR[42] != 0)); CR[42] = (CR[42] & (CR[42] - 1))) {
																																																								LS[42] = (CR[42] & (-CR[42]));
																																																								Rows[4] |= LS[42];
																																																								Columns[6] |= LS[42];
																																																								for (CR[58] = AllN ^ (Rows[6] | Columns[4] | OC[LS[42]]); ((CR[58] != 0)); CR[58] = (CR[58] & (CR[58] - 1))) {
																																																									LS[58] = (CR[58] & (-CR[58]));
																																																									Rows[6] |= LS[58];
																																																									Columns[4] |= LS[58];
																																																									OC[LS[58]] |= LS[42];
																																																									OC[LS[42]] |= LS[58];
																																																									for (CR[43] = AllN ^ (Rows[4] | Columns[7]); ((CR[43] != 0)); CR[43] = (CR[43] & (CR[43] - 1))) {
																																																										LS[43] = (CR[43] & (-CR[43]));
																																																										Rows[4] |= LS[43];
																																																										Columns[7] |= LS[43];
																																																										for (CR[67] = AllN ^ (Rows[7] | Columns[4] | OC[LS[43]]); ((CR[67] != 0)); CR[67] = (CR[67] & (CR[67] - 1))) {
																																																											LS[67] = (CR[67] & (-CR[67]));
																																																											Rows[7] |= LS[67];
																																																											Columns[4] |= LS[67];
																																																											OC[LS[67]] |= LS[43];
																																																											OC[LS[43]] |= LS[67];
																																																											CR[44] = AllN ^ (Rows[4] | Columns[8]);
																																																											if ((CR[44] != 0)) {
																																																												LS[44] = (CR[44] & (-CR[44]));
																																																												Columns[8] |= LS[44];
																																																												CR[76] = AllN ^ (Rows[8] | Columns[4] | OC[LS[44]]);
																																																												if ((CR[76] != 0)) {
																																																													LS[76] = (CR[76] & (-CR[76]));
																																																													Rows[8] |= LS[76];
																																																													OC[LS[76]] |= LS[44];
																																																													OC[LS[44]] |= LS[76];
																																																													for (CR[51] = AllN ^ (Rows[5] | Columns[6]); ((CR[51] != 0)); CR[51] = (CR[51] & (CR[51] - 1))) {
																																																														LS[51] = (CR[51] & (-CR[51]));
																																																														Rows[5] |= LS[51];
																																																														Columns[6] |= LS[51];
																																																														for (CR[59] = AllN ^ (Rows[6] | Columns[5] | OC[LS[51]]); ((CR[59] != 0)); CR[59] = (CR[59] & (CR[59] - 1))) {
																																																															LS[59] = (CR[59] & (-CR[59]));
																																																															Rows[6] |= LS[59];
																																																															Columns[5] |= LS[59];
																																																															OC[LS[59]] |= LS[51];
																																																															OC[LS[51]] |= LS[59];
																																																															for (CR[52] = AllN ^ (Rows[5] | Columns[7]); ((CR[52] != 0)); CR[52] = (CR[52] & (CR[52] - 1))) {
																																																																LS[52] = (CR[52] & (-CR[52]));
																																																																Rows[5] |= LS[52];
																																																																Columns[7] |= LS[52];
																																																																for (CR[68] = AllN ^ (Rows[7] | Columns[5] | OC[LS[52]]); ((CR[68] != 0)); CR[68] = (CR[68] & (CR[68] - 1))) {
																																																																	LS[68] = (CR[68] & (-CR[68]));
																																																																	Rows[7] |= LS[68];
																																																																	Columns[5] |= LS[68];
																																																																	OC[LS[68]] |= LS[52];
																																																																	OC[LS[52]] |= LS[68];
																																																																	CR[53] = AllN ^ (Rows[5] | Columns[8]);
																																																																	if ((CR[53] != 0)) {
																																																																		LS[53] = (CR[53] & (-CR[53]));
																																																																		Columns[8] |= LS[53];
																																																																		CR[77] = AllN ^ (Rows[8] | Columns[5] | OC[LS[53]]);
																																																																		if ((CR[77] != 0)) {
																																																																			LS[77] = (CR[77] & (-CR[77]));
																																																																			Rows[8] |= LS[77];
																																																																			OC[LS[77]] |= LS[53];
																																																																			OC[LS[53]] |= LS[77];
																																																																			for (CR[61] = AllN ^ (Rows[6] | Columns[7]); ((CR[61] != 0)); CR[61] = (CR[61] & (CR[61] - 1))) {
																																																																				LS[61] = (CR[61] & (-CR[61]));
																																																																				Rows[6] |= LS[61];
																																																																				Columns[7] |= LS[61];
																																																																				for (CR[69] = AllN ^ (Rows[7] | Columns[6] | OC[LS[61]]); ((CR[69] != 0)); CR[69] = (CR[69] & (CR[69] - 1))) {
																																																																					LS[69] = (CR[69] & (-CR[69]));
																																																																					Rows[7] |= LS[69];
																																																																					Columns[6] |= LS[69];
																																																																					OC[LS[69]] |= LS[61];
																																																																					OC[LS[61]] |= LS[69];
																																																																					CR[62] = AllN ^ (Rows[6] | Columns[8]);
																																																																					if ((CR[62] != 0)) {
																																																																						LS[62] = (CR[62] & (-CR[62]));
																																																																						Columns[8] |= LS[62];
																																																																						CR[78] = AllN ^ (Rows[8] | Columns[6] | OC[LS[62]]);
																																																																						if ((CR[78] != 0)) {
																																																																							LS[78] = (CR[78] & (-CR[78]));
																																																																							Rows[8] |= LS[78];
																																																																							OC[LS[78]] |= LS[62];
																																																																							OC[LS[62]] |= LS[78];
																																																																							CR[71] = AllN ^ (Rows[7] | Columns[8]);
																																																																							if ((CR[71] != 0)) {
																																																																								LS[71] = (CR[71] & (-CR[71]));
																																																																								CR[79] = AllN ^ (Rows[8] | Columns[7] | OC[LS[71]]);
																																																																								if ((CR[79] != 0)) {
																																																																									LS[79] = (CR[79] & (-CR[79]));
																																																																									OC[LS[79]] |= LS[71];
																																																																									OC[LS[71]] |= LS[79];
																																																																								//	is_SODLS(LS);
																																																																									SquaresCnt++;
																																																																									if (SquaresCnt % 10 == 0) {
																																																																										StopProf();
																																																																										cout << "Squares found: " << SquaresCnt << endl;
																																																																										cout << "Average time: " << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << " ms per square" << endl;
																																																																										cout << "Squares per second: " << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;
																																																																									}
																																																																									OC[LS[79]] ^= LS[71];
																																																																									OC[LS[71]] ^= LS[79];
																																																																								}
																																																																							}
																																																																							Rows[8] ^= LS[78];
																																																																							OC[LS[78]] ^= LS[62];
																																																																							OC[LS[62]] ^= LS[78];
																																																																						}
																																																																						Columns[8] ^= LS[62];
																																																																					}
																																																																					Rows[7] ^= LS[69];
																																																																					Columns[6] ^= LS[69];
																																																																					OC[LS[69]] ^= LS[61];
																																																																					OC[LS[61]] ^= LS[69];
																																																																				}
																																																																				Rows[6] ^= LS[61];
																																																																				Columns[7] ^= LS[61];
																																																																			}
																																																																			Rows[8] ^= LS[77];
																																																																			OC[LS[77]] ^= LS[53];
																																																																			OC[LS[53]] ^= LS[77];
																																																																		}
																																																																		Columns[8] ^= LS[53];
																																																																	}
																																																																	Rows[7] ^= LS[68];
																																																																	Columns[5] ^= LS[68];
																																																																	OC[LS[68]] ^= LS[52];
																																																																	OC[LS[52]] ^= LS[68];
																																																																}
																																																																Rows[5] ^= LS[52];
																																																																Columns[7] ^= LS[52];
																																																															}
																																																															Rows[6] ^= LS[59];
																																																															Columns[5] ^= LS[59];
																																																															OC[LS[59]] ^= LS[51];
																																																															OC[LS[51]] ^= LS[59];
																																																														}
																																																														Rows[5] ^= LS[51];
																																																														Columns[6] ^= LS[51];
																																																													}
																																																													Rows[8] ^= LS[76];
																																																													OC[LS[76]] ^= LS[44];
																																																													OC[LS[44]] ^= LS[76];
																																																												}
																																																												Columns[8] ^= LS[44];
																																																											}
																																																											Rows[7] ^= LS[67];
																																																											Columns[4] ^= LS[67];
																																																											OC[LS[67]] ^= LS[43];
																																																											OC[LS[43]] ^= LS[67];
																																																										}
																																																										Rows[4] ^= LS[43];
																																																										Columns[7] ^= LS[43];
																																																									}
																																																									Rows[6] ^= LS[58];
																																																									Columns[4] ^= LS[58];
																																																									OC[LS[58]] ^= LS[42];
																																																									OC[LS[42]] ^= LS[58];
																																																								}
																																																								Rows[4] ^= LS[42];
																																																								Columns[6] ^= LS[42];
																																																							}
																																																							Rows[5] ^= LS[49];
																																																							Columns[4] ^= LS[49];
																																																							OC[LS[49]] ^= LS[41];
																																																							OC[LS[41]] ^= LS[49];
																																																						}
																																																						Rows[4] ^= LS[41];
																																																						Columns[5] ^= LS[41];
																																																					}
																																																					Rows[8] ^= LS[75];
																																																					OC[LS[75]] ^= LS[35];
																																																					OC[LS[35]] ^= LS[75];
																																																				}
																																																				Columns[8] ^= LS[35];
																																																			}
																																																			Rows[7] ^= LS[66];
																																																			Columns[3] ^= LS[66];
																																																			OC[LS[66]] ^= LS[34];
																																																			OC[LS[34]] ^= LS[66];
																																																		}
																																																		Rows[3] ^= LS[34];
																																																		Columns[7] ^= LS[34];
																																																	}
																																																	Rows[6] ^= LS[57];
																																																	Columns[3] ^= LS[57];
																																																	OC[LS[57]] ^= LS[33];
																																																	OC[LS[33]] ^= LS[57];
																																																}
																																																Rows[3] ^= LS[33];
																																																Columns[6] ^= LS[33];
																																															}
																																															Rows[4] ^= LS[39];
																																															Columns[3] ^= LS[39];
																																															OC[LS[39]] ^= LS[31];
																																															OC[LS[31]] ^= LS[39];
																																														}
																																														Rows[3] ^= LS[31];
																																														Columns[4] ^= LS[31];
																																													}
																																													Rows[8] ^= LS[74];
																																													OC[LS[74]] ^= LS[26];
																																													OC[LS[26]] ^= LS[74];
																																												}
																																												Columns[8] ^= LS[26];
																																											}
																																											Rows[7] ^= LS[65];
																																											Columns[2] ^= LS[65];
																																											OC[LS[65]] ^= LS[25];
																																											OC[LS[25]] ^= LS[65];
																																										}
																																										Rows[2] ^= LS[25];
																																										Columns[7] ^= LS[25];
																																									}
																																									Rows[4] ^= LS[38];
																																									Columns[2] ^= LS[38];
																																									OC[LS[38]] ^= LS[22];
																																									OC[LS[22]] ^= LS[38];
																																								}
																																								Rows[2] ^= LS[22];
																																								Columns[4] ^= LS[22];
																																							}
																																							Rows[5] ^= LS[47];
																																							Columns[2] ^= LS[47];
																																							OC[LS[47]] ^= LS[23];
																																							OC[LS[23]] ^= LS[47];
																																						}
																																						Rows[2] ^= LS[23];
																																						Columns[5] ^= LS[23];
																																					}
																																					Rows[3] ^= LS[29];
																																					Columns[2] ^= LS[29];
																																					OC[LS[29]] ^= LS[21];
																																					OC[LS[21]] ^= LS[29];
																																				}
																																				Rows[2] ^= LS[21];
																																				Columns[3] ^= LS[21];
																																			}
																																			Rows[8] ^= LS[73];
																																			OC[LS[73]] ^= LS[17];
																																			OC[LS[17]] ^= LS[73];
																																		}
																																		Columns[8] ^= LS[17];
																																	}
																																	Rows[4] ^= LS[37];
																																	Columns[1] ^= LS[37];
																																	OC[LS[37]] ^= LS[13];
																																	OC[LS[13]] ^= LS[37];
																																}
																																Rows[1] ^= LS[13];
																																Columns[4] ^= LS[13];
																															}
																															Rows[6] ^= LS[55];
																															Columns[1] ^= LS[55];
																															OC[LS[55]] ^= LS[15];
																															OC[LS[15]] ^= LS[55];
																														}
																														Rows[1] ^= LS[15];
																														Columns[6] ^= LS[15];
																													}
																													Rows[5] ^= LS[46];
																													Columns[1] ^= LS[46];
																													OC[LS[46]] ^= LS[14];
																													OC[LS[14]] ^= LS[46];
																												}
																												Rows[1] ^= LS[14];
																												Columns[5] ^= LS[14];
																											}
																											Rows[3] ^= LS[28];
																											Columns[1] ^= LS[28];
																											OC[LS[28]] ^= LS[12];
																											OC[LS[12]] ^= LS[28];
																										}
																										Rows[1] ^= LS[12];
																										Columns[3] ^= LS[12];
																									}
																									Rows[2] ^= LS[19];
																									Columns[1] ^= LS[19];
																									OC[LS[19]] ^= LS[11];
																									OC[LS[11]] ^= LS[19];
																								}
																								Rows[1] ^= LS[11];
																								Columns[2] ^= LS[11];
																							}
																							Rows[5] ^= LS[48];
																							Columns[3] ^= LS[48];
																							OC[LS[48]] ^= LS[32];
																							OC[LS[32]] ^= LS[48];
																						}
																						Rows[3] ^= LS[32];
																						Columns[5] ^= LS[32];
																						AD ^= LS[32];
																					}
																					Rows[6] ^= LS[56];
																					Columns[2] ^= LS[56];
																					AD ^= LS[56];
																					OC[LS[56]] ^= LS[24];
																					OC[LS[24]] ^= LS[56];
																				}
																				Rows[2] ^= LS[24];
																				Columns[6] ^= LS[24];
																				AD ^= LS[24];
																			}
																			Rows[7] ^= LS[64];
																			Columns[1] ^= LS[64];
																			AD ^= LS[64];
																			OC[LS[64]] ^= LS[16];
																			OC[LS[16]] ^= LS[64];
																		}
																		Rows[1] ^= LS[16];
																		Columns[7] ^= LS[16];
																		AD ^= LS[16];
																	}
																	Rows[8] ^= LS[72];
																	AD ^= LS[72];
																	OC[LS[72]] ^= LS[8];
																	OC[LS[8]] ^= LS[72];
																}
																Rows[7] ^= LS[63];
																Columns[0] ^= LS[63];
																OC[LS[63]] ^= LS[7];
																OC[LS[7]] ^= LS[63];
															}
															Rows[6] ^= LS[54];
															Columns[0] ^= LS[54];
															OC[LS[54]] ^= LS[6];
															OC[LS[6]] ^= LS[54];
														}
														Rows[5] ^= LS[45];
														Columns[0] ^= LS[45];
														OC[LS[45]] ^= LS[5];
														OC[LS[5]] ^= LS[45];
													}
													Rows[4] ^= LS[36];
													Columns[0] ^= LS[36];
													OC[LS[36]] ^= LS[4];
													OC[LS[4]] ^= LS[36];
												}
												Rows[3] ^= LS[27];
												Columns[0] ^= LS[27];
												OC[LS[27]] ^= LS[3];
												OC[LS[3]] ^= LS[27];
											}
											Rows[2] ^= LS[18];
											Columns[0] ^= LS[18];
											OC[LS[18]] ^= LS[2];
											OC[LS[2]] ^= LS[18];
										}
										Rows[1] ^= LS[9];
										Columns[0] ^= LS[9];
										OC[LS[9]] ^= LS[1];
										OC[LS[1]] ^= LS[9];
									}
									
									Rows[8] ^= LS[80];
									Columns[8] ^= LS[80];
									OC[LS[80]] ^= LS[80];
								}
								Rows[7] ^= LS[70];
								Columns[7] ^= LS[70];
								MD ^= LS[70];
								OC[LS[70]] ^= LS[70];
							}
							Rows[6] ^= LS[60];
							Columns[6] ^= LS[60];
							MD ^= LS[60];
							OC[LS[60]] ^= LS[60];
						}
						Rows[5] ^= LS[50];
						Columns[5] ^= LS[50];
						MD ^= LS[50];
						OC[LS[50]] ^= LS[50];
					}
					Rows[4] ^= LS[40];
					Columns[4] ^= LS[40];
					MD ^= LS[40];
					AD ^= LS[40];
					OC[LS[40]] ^= LS[40];
				}
				Rows[3] ^= LS[30];
				Columns[3] ^= LS[30];
				MD ^= LS[30];
				OC[LS[30]] ^= LS[30];
			}
			Rows[2] ^= LS[20];
			Columns[2] ^= LS[20];
			MD ^= LS[20];
			OC[LS[20]] ^= LS[20];
		}
		Rows[1] ^= LS[10];
		Columns[1] ^= LS[10];
		MD ^= LS[10];
		OC[LS[10]] ^= LS[10];
	}
}
*/
void main()
{
	
	for (int i = 0; i < 10; i++) { trf[1 << i] = i; }

	double t1 = cpuTime();
	//Reset();

	vector<vector<int>> PR9 = gen_prefixes();	

	//cout << PR9.size() << endl;
	vector<int> counts(PR9.size());
	//getchar();
	
	#pragma omp parallel for
	for (int i = 0; i < PR9.size(); i++) {
//		Reset();
		counts[i]=process_prefix(PR9[i]);	
		
		if ((cur_cnt  > 0)&(cur_cnt % 5 == 0)) {
			cout << "Current progress: " << cur_cnt << "out of " << PR9.size() << endl;
		}
	}

	cout << "Final count \n";
	int sum = 0;
	for (int i = 0; i < counts.size(); i++) {
		sum += counts[i];
	
	}
	cout << sum << endl;
	getchar();

	//Generate();

	double t2 = cpuTime();
	bool OC_fine = true;
	//cout << "Total squares count: " << SquaresCnt << endl;
	cout << "Time spent: " << t2 - t1 << endl;
	cout << "OK";
	getchar();
}
