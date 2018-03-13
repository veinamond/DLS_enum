#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <iterator>
#include <omp.h>
static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
using namespace std;
const int N = 9;
const uint32_t MaxN = 1 << N;
const uint32_t AllN = MaxN - 1;

uint64_t cur_cnt = 0;

vector<vector<int>> TR2 = {
	{ 0,0,0,0 },{ 1,0,0,0 },{ 0,1,0,0 },{ 1,1,0,0 },
	{ 0,0,1,0 },{ 1,0,1,0 },{ 0,1,1,0 },{ 1,1,1,0 },
	{ 0,0,0,1 },{ 1,0,0,1 },{ 0,1,0,1 },{ 1,1,0,1 },
	{ 0,0,1,1 },{ 1,0,1,1 },{ 0,1,1,1 },{ 1,1,1,1 }
};

vector<vector<int>> TR2_X = {
	{ 0,0,0,0 },{ 0,1,0,0 },
	{ 0,0,1,0 },{ 0,1,1,0 },
	{ 0,0,0,1 },{ 0,1,0,1 },
	{ 0,0,1,1 },{ 0,1,1,1 }
};


vector<vector<int>> TR4 = {
	{ 1,2,3 },{ 1,3,2 },{ 2,1,3 },{ 2,3,1 },{ 3,1,2 },{ 3,2,1 }
};

int* trf3 = new int[1 << N];

inline void print_LSM(vector<int> LSM) {
	//0  1  2  3  4  5  6  7  9 14 18 21 27 28 35 36 42 45 49 54 56 57 58 59 60 61 62 63
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
	for (int i = 0; i < N; i++) {
		cout << trf3[LSM[i]] << " ";
	}
	cout << endl;
	int k = N;
	cout << "  " << trf3[LSM[N + 0]] << "           " << trf3[LSM[N + 1]] << endl;
	cout << "    " << trf3[LSM[N + 2]] << "       " << trf3[LSM[N + 3]] << endl;
	cout << "      " << trf3[LSM[N + 4]] << "   " << trf3[LSM[N + 5]] << endl;
	cout << "        " << trf3[LSM[N + 6]] << endl;
	cout << "      " << trf3[LSM[N + 7]] << "   " << trf3[LSM[N + 8]] << endl;
	cout << "    " << trf3[LSM[N + 9]] << "       " << trf3[LSM[N + 10]] << endl;
	cout << "  " << trf3[LSM[N + 11]] << "           " << trf3[LSM[N + 12]] << endl;

	if (LSM.size()>24) {
		for (int i = 0; i < N; i++) {
			cout << trf3[LSM[N + 13 + i]] << " ";
		}
	}
	else {
		cout << trf3[LSM[N + 13]] << "               " << trf3[LSM[N + 14]] << endl;
	}
	cout << endl;
	cout << endl;
}

inline void flip_horizontal_H(vector<int> & LSM) {
	vector<int> t(LSM);
	for (int i = 0; i < N; i++) {
		swap(LSM[i], LSM[N + 13 + i]);
	}
	swap(LSM[N + 0], LSM[N + 11]);	swap(LSM[N + 1], LSM[N + 12]);
	swap(LSM[N + 2], LSM[N + 9]);	swap(LSM[N + 3], LSM[N + 10]);
	swap(LSM[N + 4], LSM[N + 7]);	swap(LSM[N + 5], LSM[N + 8]);
}

inline void flip_vertical_X(vector<int> & LSM) {

	for (int i = 0; i < N / 2; i++) {
		swap(LSM[i], LSM[N - 1 - i]);
		//swap(LSM[N + 12 + i], LSM[12 + N + N - 1 - i]);
	}
	swap(LSM[N + 0], LSM[N + 1]);
	swap(LSM[N + 2], LSM[N + 3]);
	swap(LSM[N + 4], LSM[N + 5]);
	swap(LSM[N + 7], LSM[N + 8]);
	swap(LSM[N + 9], LSM[N + 10]);
	swap(LSM[N + 11], LSM[N + 12]);
	swap(LSM[N + 13], LSM[N + 14]);
}

inline void flip_vertical_H(vector<int> & LSM) {

	for (int i = 0; i < N / 2; i++) {
		swap(LSM[i], LSM[N - 1 - i]);
		//swap(LSM[N + 12 + i], LSM[12 + N + N - 1 - i]);
	}
	swap(LSM[N + 0], LSM[N + 1]);
	swap(LSM[N + 2], LSM[N + 3]);
	swap(LSM[N + 4], LSM[N + 5]);
	swap(LSM[N + 7], LSM[N + 8]);
	swap(LSM[N + 9], LSM[N + 10]);
	swap(LSM[N + 11], LSM[N + 12]);
	for (int i = 0; i < N / 2; i++) {
		swap(LSM[N + 13 + i], LSM[N + 13 + N - 1 - i]);
		//swap(LSM[N + 12 + i], LSM[12 + N + N - 1 - i]);
	}
}


inline void normalize(vector<int> & LSM) {
	vector<int> subst(N);
	for (int i = 0; i < N; i++) {
		subst[trf3[LSM[i]]] = i;
	}
	for (int i = 0; i < LSM.size(); i++) {
		LSM[i] = 1 << subst[trf3[LSM[i]]];

	}
}

static inline void Transform2_x(vector<int> & LSM, vector<int> tr2) {
	if (tr2[0] == 1) {
		cout << "Error!" << endl;
		getchar();
		/*	for (int i = 0; i < N; i++) {
		swap(LSM[i], LSM[N + 12 + i]);
		}
		swap(LSM[0], LSM[N - 1]);
		swap(LSM[N + 12], LSM[N + 12 + N - 1]);*/
	}
	//print_LSM(LSM);
	if (tr2[1] == 1) {
		swap(LSM[1], LSM[N - 1 - 1]);
		swap(LSM[N + 0], LSM[N + 12]);
		swap(LSM[N + 1], LSM[N + 11]);
	}
	//print_LSM(LSM);
	if (tr2[2] == 1) {
		swap(LSM[2], LSM[N - 1 - 2]);
		swap(LSM[N + 2], LSM[N + 10]);
		swap(LSM[N + 3], LSM[N + 9]);
	}
	//print_LSM(LSM);
	if (tr2[3] == 1) {
		swap(LSM[3], LSM[N - 1 - 3]);
		swap(LSM[N + 4], LSM[N + 8]);
		swap(LSM[N + 5], LSM[N + 7]);
	}
	//print_LSM(LSM);
}
static inline void Transform2_h(vector<int> & LSM, vector<int> tr2) {
	if (tr2[0] == 1) {
		for (int i = 0; i < N; i++) {
			swap(LSM[i], LSM[N + 13 + i]);
		}
		swap(LSM[0], LSM[N - 1]);
		swap(LSM[N + 13], LSM[N + 13 + N - 1]);
	}
	//print_LSM(LSM);
	if (tr2[1] == 1) {
		swap(LSM[1], LSM[N - 1 - 1]);
		swap(LSM[N + 13 + 1], LSM[N + 13 + N - 1 - 1]);
		swap(LSM[N + 0], LSM[N + 12]);
		swap(LSM[N + 1], LSM[N + 11]);
	}
	//print_LSM(LSM);
	if (tr2[2] == 1) {
		swap(LSM[2], LSM[N - 1 - 2]);
		swap(LSM[N + 13 + 2], LSM[N + 13 + N - 1 - 2]);
		swap(LSM[N + 2], LSM[N + 10]);
		swap(LSM[N + 3], LSM[N + 9]);
	}
	//print_LSM(LSM);
	if (tr2[3] == 1) {
		swap(LSM[3], LSM[N - 1 - 3]);
		swap(LSM[N + 13 + 3], LSM[N + 13 + N - 1 - 3]);
		swap(LSM[N + 4], LSM[N + 8]);
		swap(LSM[N + 5], LSM[N + 7]);
	}
	//print_LSM(LSM);
}


static inline void Transform4_x(vector<int> & LSM, vector<int> tr4) {


	vector<int> t(LSM);
	for (int i = 0; i < tr4.size(); i++) {
		t[i + 1] = LSM[tr4[i]];

		t[N - 1 - (i + 1)] = LSM[N - 1 - tr4[i]];

		//t[N + 13 + i + 1] = LSM[N + 13 + tr4[i]];
		//t[N + 13 + N - 1 - (i + 1)] = LSM[N + 13 + N - 1 - tr4[i]];

		t[N + 2 * i] = LSM[N + 2 * (tr4[i] - 1)];
		t[N + 2 * i + 1] = LSM[N + 2 * (tr4[i] - 1) + 1];

		t[N + 13 - (2 * (i + 1))] = LSM[N + 13 - (2 * (tr4[i]))];
		t[N + 13 - (2 * (i + 1)) + 1] = LSM[N + 13 - (2 * (tr4[i])) + 1];

	}
	LSM = t;
	//print_LSM(LSM);
}



static inline void Transform4_h(vector<int> & LSM, vector<int> tr4) {
	vector<int> t(LSM);
	for (int i = 0; i < tr4.size(); i++) {
		t[i + 1] = LSM[tr4[i]];

		t[N - 1 - (i + 1)] = LSM[N - 1 - tr4[i]];

		t[N + 13 + i + 1] = LSM[N + 13 + tr4[i]];
		t[N + 13 + N - 1 - (i + 1)] = LSM[N + 13 + N - 1 - tr4[i]];

		t[N + 2 * i] = LSM[N + 2 * (tr4[i] - 1)];
		t[N + 2 * i + 1] = LSM[N + 2 * (tr4[i] - 1) + 1];

		t[N + 13 - (2 * (i + 1))] = LSM[N + 13 - (2 * (tr4[i]))];
		t[N + 13 - (2 * (i + 1)) + 1] = LSM[N + 13 - (2 * (tr4[i])) + 1];

	}
	LSM = t;
	//print_LSM(LSM);
}


int PROCESS_LSM_X(vector<int> LSM_X) {
	//vector<vector<int>> ST;
	set<vector<int>> ST;
	bool r = true;
	for (int i = 0; i < TR2_X.size(); i++) {
		for (int j = 0; j < TR4.size(); j++) {
			vector<int> t(LSM_X);
			Transform2_x(t, TR2_X[i]);
			Transform4_x(t, TR4[j]);

			//vector<int> th(t);
			//flip_horizontal(th);

			//vector<int> tv(t);
			//flip_vertical(tv);

			//vector<int> thv(th);
			//flip_vertical(thv);


			normalize(t);
			//normalize(th);
			//normalize(tv);
			//normalize(thv);				

			ST.insert(t);
			//ST.insert(th);
			//ST.insert(tv);
			//ST.insert(thv);
			if (*ST.begin() != LSM_X) {
				r = false;
				return 0;
			}
		}
	}
	if (r == true) {
		return ST.size();
	}
}



int PROCESS_LSM_H(vector<int> LSM_H) {
	//vector<vector<int>> ST;
	set<vector<int>> ST;
	bool r = true;
	for (int i = 0; i < TR2.size(); i++) {
		for (int j = 0; j < TR4.size(); j++) {
			vector<int> t(LSM_H);
			Transform2_h(t, TR2[i]);
			Transform4_h(t, TR4[j]);

			//vector<int> th(t);
			//flip_horizontal(th);

			vector<int> tv(t);
			flip_vertical_H(tv);

			//vector<int> thv(th);
			//flip_vertical(thv);


			normalize(t);
			//normalize(th);
			normalize(tv);
			//normalize(thv);				

			ST.insert(t);
			//ST.insert(th);
			ST.insert(tv);
			//ST.insert(thv);
			if (*ST.begin() != LSM_H) {
				r = false;
				return 0;
			}
		}
	}
	if (r == true) {
		return ST.size();
	}
}




vector<vector<int>> Generate_WU() {

	vector<vector<int>> RES;
	int32_t LS[N*N];
	int32_t CR[N*N];

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
	MD |= LS[0];
	AD |= LS[N - 1];

	//9 14 18 21 27 28 35 36 42 45 49 54 56 57 58 59 60 61 62 63
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19

	for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); CR[40] != 0; CR[40] = (CR[40] & (CR[40] - 1))) {
		LS[40] = (CR[40] & (-CR[40]));
		//{
		//LS[40] = WU[0];
		Rows[4] |= LS[40];
		Columns[4] |= LS[40];
		MD |= LS[40];
		AD |= LS[40];
		for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); CR[10] != 0; CR[10] = (CR[10] & (CR[10] - 1))) {
			LS[10] = (CR[10] & (-CR[10]));
			//{
			//LS[10] = WU[1];
			Rows[1] |= LS[10];
			Columns[1] |= LS[10];
			MD |= LS[10];
			for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); CR[16] != 0; CR[16] = (CR[16] & (CR[16] - 1))) {
				LS[16] = (CR[16] & (-CR[16]));
				//{
				//LS[16] = WU[2];
				Rows[1] |= LS[16];
				Columns[7] |= LS[16];
				AD |= LS[16];
				for (CR[64] = AllN ^ (Rows[7] | Columns[1] | AD); CR[64] != 0; CR[64] = (CR[64] & (CR[64] - 1))) {
					LS[64] = (CR[64] & (-CR[64]));
					//{
					//LS[64] = WU[3];
					Rows[7] |= LS[64];
					Columns[1] |= LS[64];
					AD |= LS[64];
					for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); CR[70] != 0; CR[70] = (CR[70] & (CR[70] - 1))) {
						LS[70] = (CR[70] & (-CR[70]));
						//{
						//LS[70] = WU[4];
						Rows[7] |= LS[70];
						Columns[7] |= LS[70];
						MD |= LS[70];
						for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); CR[20] != 0; CR[20] = (CR[20] & (CR[20] - 1))) {
							LS[20] = (CR[20] & (-CR[20]));
							//{
							//LS[20] = WU[5];
							Rows[2] |= LS[20];
							Columns[2] |= LS[20];
							MD |= LS[20];
							for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); CR[24] != 0; CR[24] = (CR[24] & (CR[24] - 1))) {
								LS[24] = (CR[24] & (-CR[24]));
								//{
								//	LS[24] = WU[6];
								Rows[2] |= LS[24];
								Columns[6] |= LS[24];
								AD |= LS[24];
								for (CR[56] = AllN ^ (Rows[6] | Columns[2] | AD); CR[56] != 0; CR[56] = (CR[56] & (CR[56] - 1))) {
									LS[56] = (CR[56] & (-CR[56]));
									//{
									//LS[56] = WU[7];
									Rows[6] |= LS[56];
									Columns[2] |= LS[56];
									AD |= LS[56];
									for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); CR[60] != 0; CR[60] = (CR[60] & (CR[60] - 1))) {
										LS[60] = (CR[60] & (-CR[60]));
										//{
										//	LS[60] = WU[8];
										Rows[6] |= LS[60];
										Columns[6] |= LS[60];
										MD |= LS[60];
										for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); CR[30] != 0; CR[30] = (CR[30] & (CR[30] - 1))) {
											LS[30] = (CR[30] & (-CR[30]));
											//{										
											//LS[30] = WU[9];

											vector<int> t = { LS[40],LS[10], LS[16], LS[64], LS[70], LS[20], LS[24], LS[56], LS[60], LS[30] };
											RES.push_back(t);

										}
										Rows[6] ^= LS[60];
										Columns[6] ^= LS[60];
										MD ^= LS[60];
									}
									Rows[6] ^= LS[56];
									Columns[2] ^= LS[56];
									AD ^= LS[56];
								}
								Rows[2] ^= LS[24];
								Columns[6] ^= LS[24];
								AD ^= LS[24];
							}
							Rows[2] ^= LS[20];
							Columns[2] ^= LS[20];
							MD ^= LS[20];
						}
						Rows[7] ^= LS[70];
						Columns[7] ^= LS[70];
						MD ^= LS[70];
					}
					Rows[7] ^= LS[64];
					Columns[1] ^= LS[64];
					AD ^= LS[64];
				}
				Rows[1] ^= LS[16];
				Columns[7] ^= LS[16];
				AD ^= LS[16];
			}
			Rows[1] ^= LS[10];
			Columns[1] ^= LS[10];
			MD ^= LS[10];
		}
		Rows[4] ^= LS[40];
		Columns[4] ^= LS[40];
		MD ^= LS[40];
		AD ^= LS[40];
	}

	return RES;
	//	return SquaresCnt;
}
uint64_t Generate(vector<int> WU);
uint64_t Generate_rc_X();
uint64_t Generate_rc_H(vector<int> WU);

void main()
{
	for (int i = 0; i < N; i++) { trf3[1 << i] = i; }
	vector<vector<int>> WUS = Generate_WU();
	cout << "WU SIZE:" << WUS.size() << endl;

	//char FileData[11] = { '1','2','0','3','6','5','2','6','4','7',NULL };
	vector<int> p = { 1,2,0,3,4,3,2,4,5,6 };
	vector<int> test_WU(p);
	
	//vector<int> test_WU = { 1 << 1, 1 << 2, 1 << 0, 1 << 3, 1 << 6, 1 << 5, 1 << 2, 1 << 6, 1 << 4, 1 << 7 };
	//Count: 844646272
	//Time spent : 160.496	
	//	Generate_rc_X();
//#pragma omp parallel for
	cout << Generate_rc_H(WUS[100000]) << endl;
	for (int i =0; i <WUS.size();i++){
		double ct0 = cpuTime();
		uint64_t c_res=Generate_rc_H(WUS[i]);		

		double ct1= cpuTime();
		if (c_res != 0) {
			cout<<"Workunit "<<i<<": "<<c_res<<endl;
			cout << "Time:" << ct1 - ct0 << endl;
		}
		if (cur_cnt % 10000 == 0) {
			cout << "Processed " << cur_cnt << " out of " << WUS.size() << endl;
		}
	}






	getchar();

}
uint64_t Generate(vector<int> WU)
{

	uint64_t SquaresCnt = 0;
	int32_t LS[N*N];
	int32_t CR[N*N];
	uint64_t multiple = 0;
	uint64_t TotalCnt = 0;
	uint64_t processed = 0;
	uint64_t k = 0;

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
	MD |= LS[0];
	AD |= LS[N - 1];

	//9 14 18 21 27 28 35 36 42 45 49 54 56 57 58 59 60 61 62 63
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19

	//	for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); CR[40] != 0; CR[40] = (CR[40] & (CR[40] - 1))) {
	//	LS[40] = (CR[40] & (-CR[40]));
	{
		LS[40] = WU[0];
		Rows[4] |= LS[40];
		Columns[4] |= LS[40];
		MD |= LS[40];
		AD |= LS[40];
		//for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); CR[10] != 0; CR[10] = (CR[10] & (CR[10] - 1))) {
		//LS[10] = (CR[10] & (-CR[10]));
		{
			LS[10] = WU[1];
			Rows[1] |= LS[10];
			Columns[1] |= LS[10];
			MD |= LS[10];
			//for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); CR[16] != 0; CR[16] = (CR[16] & (CR[16] - 1))) {
			//LS[16] = (CR[16] & (-CR[16]));
			{
				LS[16] = WU[2];
				Rows[1] |= LS[16];
				Columns[7] |= LS[16];
				AD |= LS[16];
				//for (CR[64] = AllN ^ (Rows[7] | Columns[1] | AD); CR[64] != 0; CR[64] = (CR[64] & (CR[64] - 1))) {
				//LS[64] = (CR[64] & (-CR[64]));
				{
					LS[64] = WU[3];
					Rows[7] |= LS[64];
					Columns[1] |= LS[64];
					AD |= LS[64];
					//for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); CR[70] != 0; CR[70] = (CR[70] & (CR[70] - 1))) {
					//LS[70] = (CR[70] & (-CR[70]));
					{
						LS[70] = WU[4];
						Rows[7] |= LS[70];
						Columns[7] |= LS[70];
						MD |= LS[70];
						//for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); CR[20] != 0; CR[20] = (CR[20] & (CR[20] - 1))) {
						//LS[20] = (CR[20] & (-CR[20]));
						{
							LS[20] = WU[5];
							Rows[2] |= LS[20];
							Columns[2] |= LS[20];
							MD |= LS[20];
							//for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); CR[24] != 0; CR[24] = (CR[24] & (CR[24] - 1))) {
							//	LS[24] = (CR[24] & (-CR[24]));
							{
								LS[24] = WU[6];
								Rows[2] |= LS[24];
								Columns[6] |= LS[24];
								AD |= LS[24];
								//for (CR[56] = AllN ^ (Rows[6] | Columns[2] | AD); CR[56] != 0; CR[56] = (CR[56] & (CR[56] - 1))) {
								//LS[56] = (CR[56] & (-CR[56]));
								{
									LS[56] = WU[7];
									Rows[6] |= LS[56];
									Columns[2] |= LS[56];
									AD |= LS[56];
									//for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); CR[60] != 0; CR[60] = (CR[60] & (CR[60] - 1))) {
									//	LS[60] = (CR[60] & (-CR[60]));
									{
										LS[60] = WU[8];
										Rows[6] |= LS[60];
										Columns[6] |= LS[60];
										MD |= LS[60];
										//for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); CR[30] != 0; CR[30] = (CR[30] & (CR[30] - 1))) {
										//LS[30] = (CR[30] & (-CR[30]));
										{
											LS[30] = WU[9];
											Rows[3] |= LS[30];
											Columns[3] |= LS[30];
											MD |= LS[30];

											for (CR[32] = AllN ^ (Rows[3] | Columns[5] | AD); CR[32] != 0; CR[32] = (CR[32] & (CR[32] - 1))) {
												LS[32] = (CR[32] & (-CR[32]));
												Rows[3] |= LS[32];
												Columns[5] |= LS[32];
												AD |= LS[32];
												for (CR[48] = AllN ^ (Rows[5] | Columns[3] | AD); CR[48] != 0; CR[48] = (CR[48] & (CR[48] - 1))) {
													LS[48] = (CR[48] & (-CR[48]));
													Rows[5] |= LS[48];
													Columns[3] |= LS[48];
													AD |= LS[48];
													CR[72] = AllN ^ (Rows[8] | Columns[0] | AD);
													if (CR[72] != 0) {
														LS[72] = (CR[72] & (-CR[72]));
														Rows[8] |= LS[72];
														Columns[0] |= LS[72];
														for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); CR[50] != 0; CR[50] = (CR[50] & (CR[50] - 1))) {
															LS[50] = (CR[50] & (-CR[50]));
															Rows[5] |= LS[50];
															Columns[5] |= LS[50];
															MD |= LS[50];
															CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
															if (CR[80] != 0) {
																LS[80] = (CR[80] & (-CR[80]));
																vector<int> LSM_X = { LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																	LS[10],LS[16],
																	LS[20],LS[24],
																	LS[30],LS[32],
																	LS[40],
																	LS[48],LS[50],
																	LS[56],LS[60],
																	LS[64],LS[70],
																	LS[72],LS[80]
																};

																k++;
																TotalCnt += SquaresCnt*multiple;
																multiple = PROCESS_LSM_X(LSM_X);
																SquaresCnt = 0;
																if (multiple != 0) {
																	processed++;


																	Rows[8] |= LS[80];
																	Columns[8] |= LS[80];

																	/*print_LSM(LSM_X);
																	cout << "Flip vertical\n";
																	flip_vertical_X(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Normalize\n";
																	normalize(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Transform 2 {0,1,1,1}\n";
																	Transform2_x(LSM_X, { 0,1,1,1 });
																	print_LSM(LSM_X);

																	cout << "Normalize\n";
																	normalize(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Transform 4 {3,1,2}\n";
																	Transform4_x(LSM_X, { 3,1,2 });
																	print_LSM(LSM_X);


																	getchar();
																	*/

																	for (CR[73] = AllN ^ (Rows[8] | Columns[1]); CR[73] != 0; CR[73] = (CR[73] & (CR[73] - 1))) {
																		LS[73] = (CR[73] & (-CR[73]));
																		Rows[8] |= LS[73];
																		Columns[1] |= LS[73];
																		for (CR[74] = AllN ^ (Rows[8] | Columns[2]); CR[74] != 0; CR[74] = (CR[74] & (CR[74] - 1))) {
																			LS[74] = (CR[74] & (-CR[74]));
																			Rows[8] |= LS[74];
																			Columns[2] |= LS[74];
																			for (CR[75] = AllN ^ (Rows[8] | Columns[3]); CR[75] != 0; CR[75] = (CR[75] & (CR[75] - 1))) {
																				LS[75] = (CR[75] & (-CR[75]));
																				Rows[8] |= LS[75];
																				Columns[3] |= LS[75];
																				for (CR[76] = AllN ^ (Rows[8] | Columns[4]); CR[76] != 0; CR[76] = (CR[76] & (CR[76] - 1))) {
																					LS[76] = (CR[76] & (-CR[76]));
																					Rows[8] |= LS[76];
																					Columns[4] |= LS[76];
																					for (CR[77] = AllN ^ (Rows[8] | Columns[5]); CR[77] != 0; CR[77] = (CR[77] & (CR[77] - 1))) {
																						LS[77] = (CR[77] & (-CR[77]));
																						Rows[8] |= LS[77];
																						Columns[5] |= LS[77];
																						for (CR[78] = AllN ^ (Rows[8] | Columns[6]); CR[78] != 0; CR[78] = (CR[78] & (CR[78] - 1))) {
																							LS[78] = (CR[78] & (-CR[78]));
																							Rows[8] |= LS[78];
																							Columns[6] |= LS[78];
																							CR[79] = AllN ^ (Rows[8] | Columns[7]);
																							if (CR[79] != 0) {
																								LS[79] = (CR[79] & (-CR[79]));

																								/*		vector<int> LSM_H = {LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																								LS[10],LS[16],
																								LS[20],LS[24],
																								LS[30],LS[32],
																								LS[40],
																								LS[48],LS[50],
																								LS[56],LS[60],
																								LS[64],LS[70],
																								LS[72],LS[73],LS[74],LS[75],LS[76],LS[77],LS[78],LS[79],LS[80]
																								};*/

																								/*print_LSM(LSM_H);
																								cout << "Flip vertical\n";
																								flip_vertical_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Flip horizontal\n";
																								flip_horizontal_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 2 {0,1,1,1}\n";
																								Transform2_h(LSM_H, { 1,1,1,1 });
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 4 {3,1,2}\n";
																								Transform4_h(LSM_H, { 3,1,2 });
																								print_LSM(LSM_H);
																								getchar();
																								*/

																								Columns[7] |= LS[79];
																								for (CR[11] = AllN ^ (Rows[1] | Columns[2]); CR[11] != 0; CR[11] = (CR[11] & (CR[11] - 1))) {
																									LS[11] = (CR[11] & (-CR[11]));
																									Rows[1] |= LS[11];
																									Columns[2] |= LS[11];
																									for (CR[12] = AllN ^ (Rows[1] | Columns[3]); CR[12] != 0; CR[12] = (CR[12] & (CR[12] - 1))) {
																										LS[12] = (CR[12] & (-CR[12]));
																										Rows[1] |= LS[12];
																										Columns[3] |= LS[12];
																										for (CR[14] = AllN ^ (Rows[1] | Columns[5]); CR[14] != 0; CR[14] = (CR[14] & (CR[14] - 1))) {
																											LS[14] = (CR[14] & (-CR[14]));
																											Rows[1] |= LS[14];
																											Columns[5] |= LS[14];
																											for (CR[15] = AllN ^ (Rows[1] | Columns[6]); CR[15] != 0; CR[15] = (CR[15] & (CR[15] - 1))) {
																												LS[15] = (CR[15] & (-CR[15]));
																												Rows[1] |= LS[15];
																												Columns[6] |= LS[15];
																												for (CR[13] = AllN ^ (Rows[1] | Columns[4]); CR[13] != 0; CR[13] = (CR[13] & (CR[13] - 1))) {
																													LS[13] = (CR[13] & (-CR[13]));
																													Rows[1] |= LS[13];
																													Columns[4] |= LS[13];
																													for (CR[9] = AllN ^ (Rows[1] | Columns[0]); CR[9] != 0; CR[9] = (CR[9] & (CR[9] - 1))) {
																														LS[9] = (CR[9] & (-CR[9]));
																														Rows[1] |= LS[9];
																														Columns[0] |= LS[9];
																														CR[17] = AllN ^ (Rows[1] | Columns[8]);
																														if (CR[17] != 0) {
																															LS[17] = (CR[17] & (-CR[17]));
																															Columns[8] |= LS[17];
																															for (CR[21] = AllN ^ (Rows[2] | Columns[3]); CR[21] != 0; CR[21] = (CR[21] & (CR[21] - 1))) {
																																LS[21] = (CR[21] & (-CR[21]));
																																Rows[2] |= LS[21];
																																Columns[3] |= LS[21];
																																for (CR[23] = AllN ^ (Rows[2] | Columns[5]); CR[23] != 0; CR[23] = (CR[23] & (CR[23] - 1))) {
																																	LS[23] = (CR[23] & (-CR[23]));
																																	Rows[2] |= LS[23];
																																	Columns[5] |= LS[23];
																																	for (CR[19] = AllN ^ (Rows[2] | Columns[1]); CR[19] != 0; CR[19] = (CR[19] & (CR[19] - 1))) {
																																		LS[19] = (CR[19] & (-CR[19]));
																																		Rows[2] |= LS[19];
																																		Columns[1] |= LS[19];
																																		for (CR[22] = AllN ^ (Rows[2] | Columns[4]); CR[22] != 0; CR[22] = (CR[22] & (CR[22] - 1))) {
																																			LS[22] = (CR[22] & (-CR[22]));
																																			Rows[2] |= LS[22];
																																			Columns[4] |= LS[22];
																																			for (CR[25] = AllN ^ (Rows[2] | Columns[7]); CR[25] != 0; CR[25] = (CR[25] & (CR[25] - 1))) {
																																				LS[25] = (CR[25] & (-CR[25]));
																																				Rows[2] |= LS[25];
																																				Columns[7] |= LS[25];
																																				for (CR[18] = AllN ^ (Rows[2] | Columns[0]); CR[18] != 0; CR[18] = (CR[18] & (CR[18] - 1))) {
																																					LS[18] = (CR[18] & (-CR[18]));
																																					Rows[2] |= LS[18];
																																					Columns[0] |= LS[18];
																																					CR[26] = AllN ^ (Rows[2] | Columns[8]);
																																					if (CR[26] != 0) {
																																						LS[26] = (CR[26] & (-CR[26]));
																																						Columns[8] |= LS[26];
																																						for (CR[57] = AllN ^ (Rows[6] | Columns[3]); CR[57] != 0; CR[57] = (CR[57] & (CR[57] - 1))) {
																																							LS[57] = (CR[57] & (-CR[57]));
																																							Rows[6] |= LS[57];
																																							Columns[3] |= LS[57];
																																							for (CR[59] = AllN ^ (Rows[6] | Columns[5]); CR[59] != 0; CR[59] = (CR[59] & (CR[59] - 1))) {
																																								LS[59] = (CR[59] & (-CR[59]));
																																								Rows[6] |= LS[59];
																																								Columns[5] |= LS[59];
																																								for (CR[55] = AllN ^ (Rows[6] | Columns[1]); CR[55] != 0; CR[55] = (CR[55] & (CR[55] - 1))) {
																																									LS[55] = (CR[55] & (-CR[55]));
																																									Rows[6] |= LS[55];
																																									Columns[1] |= LS[55];
																																									for (CR[58] = AllN ^ (Rows[6] | Columns[4]); CR[58] != 0; CR[58] = (CR[58] & (CR[58] - 1))) {
																																										LS[58] = (CR[58] & (-CR[58]));
																																										Rows[6] |= LS[58];
																																										Columns[4] |= LS[58];
																																										for (CR[61] = AllN ^ (Rows[6] | Columns[7]); CR[61] != 0; CR[61] = (CR[61] & (CR[61] - 1))) {
																																											LS[61] = (CR[61] & (-CR[61]));
																																											Rows[6] |= LS[61];
																																											Columns[7] |= LS[61];
																																											for (CR[54] = AllN ^ (Rows[6] | Columns[0]); CR[54] != 0; CR[54] = (CR[54] & (CR[54] - 1))) {
																																												LS[54] = (CR[54] & (-CR[54]));
																																												Rows[6] |= LS[54];
																																												Columns[0] |= LS[54];
																																												CR[62] = AllN ^ (Rows[6] | Columns[8]);
																																												if (CR[62] != 0) {
																																													LS[62] = (CR[62] & (-CR[62]));
																																													Columns[8] |= LS[62];
																																													for (CR[66] = AllN ^ (Rows[7] | Columns[3]); CR[66] != 0; CR[66] = (CR[66] & (CR[66] - 1))) {
																																														LS[66] = (CR[66] & (-CR[66]));
																																														Rows[7] |= LS[66];
																																														Columns[3] |= LS[66];
																																														CR[39] = AllN ^ (Rows[4] | Columns[3]);
																																														if (CR[39] != 0) {
																																															LS[39] = (CR[39] & (-CR[39]));
																																															Rows[4] |= LS[39];
																																															for (CR[68] = AllN ^ (Rows[7] | Columns[5]); CR[68] != 0; CR[68] = (CR[68] & (CR[68] - 1))) {
																																																LS[68] = (CR[68] & (-CR[68]));
																																																Rows[7] |= LS[68];
																																																Columns[5] |= LS[68];
																																																CR[41] = AllN ^ (Rows[4] | Columns[5]);
																																																if (CR[41] != 0) {
																																																	LS[41] = (CR[41] & (-CR[41]));
																																																	Rows[4] |= LS[41];
																																																	for (CR[67] = AllN ^ (Rows[7] | Columns[4]); CR[67] != 0; CR[67] = (CR[67] & (CR[67] - 1))) {
																																																		LS[67] = (CR[67] & (-CR[67]));
																																																		Rows[7] |= LS[67];
																																																		Columns[4] |= LS[67];
																																																		for (CR[63] = AllN ^ (Rows[7] | Columns[0]); CR[63] != 0; CR[63] = (CR[63] & (CR[63] - 1))) {
																																																			LS[63] = (CR[63] & (-CR[63]));
																																																			Rows[7] |= LS[63];
																																																			Columns[0] |= LS[63];
																																																			for (CR[65] = AllN ^ (Rows[7] | Columns[2]); CR[65] != 0; CR[65] = (CR[65] & (CR[65] - 1))) {
																																																				LS[65] = (CR[65] & (-CR[65]));
																																																				Rows[7] |= LS[65];
																																																				Columns[2] |= LS[65];
																																																				for (CR[69] = AllN ^ (Rows[7] | Columns[6]); CR[69] != 0; CR[69] = (CR[69] & (CR[69] - 1))) {
																																																					LS[69] = (CR[69] & (-CR[69]));
																																																					Rows[7] |= LS[69];
																																																					Columns[6] |= LS[69];
																																																					CR[71] = AllN ^ (Rows[7] | Columns[8]);
																																																					if (CR[71] != 0) {
																																																						LS[71] = (CR[71] & (-CR[71]));
																																																						Columns[8] |= LS[71];
																																																						for (CR[31] = AllN ^ (Rows[3] | Columns[4]); CR[31] != 0; CR[31] = (CR[31] & (CR[31] - 1))) {
																																																							LS[31] = (CR[31] & (-CR[31]));
																																																							Rows[3] |= LS[31];
																																																							Columns[4] |= LS[31];
																																																							CR[49] = AllN ^ (Rows[5] | Columns[4]);
																																																							if (CR[49] != 0) {
																																																								LS[49] = (CR[49] & (-CR[49]));
																																																								Rows[5] |= LS[49];
																																																								for (CR[27] = AllN ^ (Rows[3] | Columns[0]); CR[27] != 0; CR[27] = (CR[27] & (CR[27] - 1))) {
																																																									LS[27] = (CR[27] & (-CR[27]));
																																																									Rows[3] |= LS[27];
																																																									Columns[0] |= LS[27];
																																																									for (CR[28] = AllN ^ (Rows[3] | Columns[1]); CR[28] != 0; CR[28] = (CR[28] & (CR[28] - 1))) {
																																																										LS[28] = (CR[28] & (-CR[28]));
																																																										Rows[3] |= LS[28];
																																																										Columns[1] |= LS[28];
																																																										for (CR[29] = AllN ^ (Rows[3] | Columns[2]); CR[29] != 0; CR[29] = (CR[29] & (CR[29] - 1))) {
																																																											LS[29] = (CR[29] & (-CR[29]));
																																																											Rows[3] |= LS[29];
																																																											Columns[2] |= LS[29];
																																																											for (CR[33] = AllN ^ (Rows[3] | Columns[6]); CR[33] != 0; CR[33] = (CR[33] & (CR[33] - 1))) {
																																																												LS[33] = (CR[33] & (-CR[33]));
																																																												Rows[3] |= LS[33];
																																																												Columns[6] |= LS[33];
																																																												for (CR[34] = AllN ^ (Rows[3] | Columns[7]); CR[34] != 0; CR[34] = (CR[34] & (CR[34] - 1))) {
																																																													LS[34] = (CR[34] & (-CR[34]));
																																																													Rows[3] |= LS[34];
																																																													Columns[7] |= LS[34];
																																																													CR[35] = AllN ^ (Rows[3] | Columns[8]);
																																																													if (CR[35] != 0) {
																																																														LS[35] = (CR[35] & (-CR[35]));
																																																														Columns[8] |= LS[35];
																																																														for (CR[36] = AllN ^ (Rows[4] | Columns[0]); CR[36] != 0; CR[36] = (CR[36] & (CR[36] - 1))) {
																																																															LS[36] = (CR[36] & (-CR[36]));
																																																															Rows[4] |= LS[36];
																																																															Columns[0] |= LS[36];
																																																															CR[45] = AllN ^ (Rows[5] | Columns[0]);
																																																															if (CR[45] != 0) {
																																																																LS[45] = (CR[45] & (-CR[45]));
																																																																Rows[5] |= LS[45];
																																																																for (CR[37] = AllN ^ (Rows[4] | Columns[1]); CR[37] != 0; CR[37] = (CR[37] & (CR[37] - 1))) {
																																																																	LS[37] = (CR[37] & (-CR[37]));
																																																																	Rows[4] |= LS[37];
																																																																	Columns[1] |= LS[37];
																																																																	CR[46] = AllN ^ (Rows[5] | Columns[1]);
																																																																	if (CR[46] != 0) {
																																																																		LS[46] = (CR[46] & (-CR[46]));
																																																																		Rows[5] |= LS[46];
																																																																		for (CR[38] = AllN ^ (Rows[4] | Columns[2]); CR[38] != 0; CR[38] = (CR[38] & (CR[38] - 1))) {
																																																																			LS[38] = (CR[38] & (-CR[38]));
																																																																			Rows[4] |= LS[38];
																																																																			Columns[2] |= LS[38];
																																																																			CR[47] = AllN ^ (Rows[5] | Columns[2]);
																																																																			if (CR[47] != 0) {
																																																																				LS[47] = (CR[47] & (-CR[47]));
																																																																				Rows[5] |= LS[47];
																																																																				for (CR[42] = AllN ^ (Rows[4] | Columns[6]); CR[42] != 0; CR[42] = (CR[42] & (CR[42] - 1))) {
																																																																					LS[42] = (CR[42] & (-CR[42]));
																																																																					Rows[4] |= LS[42];
																																																																					Columns[6] |= LS[42];
																																																																					CR[51] = AllN ^ (Rows[5] | Columns[6]);
																																																																					if (CR[51] != 0) {
																																																																						LS[51] = (CR[51] & (-CR[51]));
																																																																						Rows[5] |= LS[51];
																																																																						for (CR[43] = AllN ^ (Rows[4] | Columns[7]); CR[43] != 0; CR[43] = (CR[43] & (CR[43] - 1))) {
																																																																							LS[43] = (CR[43] & (-CR[43]));
																																																																							Rows[4] |= LS[43];
																																																																							Columns[7] |= LS[43];
																																																																							CR[44] = AllN ^ (Rows[4] | Columns[8]);
																																																																							if (CR[44] != 0) {
																																																																								LS[44] = (CR[44] & (-CR[44]));
																																																																								Columns[8] |= LS[44];
																																																																								CR[52] = AllN ^ (Rows[5] | Columns[7]);
																																																																								if (CR[52] != 0) {
																																																																									LS[52] = (CR[52] & (-CR[52]));
																																																																									Rows[5] |= LS[52];
																																																																									CR[53] = AllN ^ (Rows[5] | Columns[8]);
																																																																									if (CR[53] != 0) {
																																																																										LS[53] = (CR[53] & (-CR[53]));
																																																																										SquaresCnt++;
																																																																										if (SquaresCnt % 10000000 == 0) {
																																																																											cout << "Squares found: " << SquaresCnt << endl;

																																																																										}
																																																																									}
																																																																									Rows[5] ^= LS[52];
																																																																								}
																																																																								Columns[8] ^= LS[44];
																																																																							}
																																																																							Rows[4] ^= LS[43];
																																																																							Columns[7] ^= LS[43];
																																																																						}
																																																																						Rows[5] ^= LS[51];
																																																																					}
																																																																					Rows[4] ^= LS[42];
																																																																					Columns[6] ^= LS[42];
																																																																				}
																																																																				Rows[5] ^= LS[47];
																																																																			}
																																																																			Rows[4] ^= LS[38];
																																																																			Columns[2] ^= LS[38];
																																																																		}
																																																																		Rows[5] ^= LS[46];
																																																																	}
																																																																	Rows[4] ^= LS[37];
																																																																	Columns[1] ^= LS[37];
																																																																}
																																																																Rows[5] ^= LS[45];
																																																															}
																																																															Rows[4] ^= LS[36];
																																																															Columns[0] ^= LS[36];
																																																														}
																																																														Columns[8] ^= LS[35];
																																																													}
																																																													Rows[3] ^= LS[34];
																																																													Columns[7] ^= LS[34];
																																																												}
																																																												Rows[3] ^= LS[33];
																																																												Columns[6] ^= LS[33];
																																																											}
																																																											Rows[3] ^= LS[29];
																																																											Columns[2] ^= LS[29];
																																																										}
																																																										Rows[3] ^= LS[28];
																																																										Columns[1] ^= LS[28];
																																																									}
																																																									Rows[3] ^= LS[27];
																																																									Columns[0] ^= LS[27];
																																																								}
																																																								Rows[5] ^= LS[49];
																																																							}
																																																							Rows[3] ^= LS[31];
																																																							Columns[4] ^= LS[31];
																																																						}
																																																						Columns[8] ^= LS[71];
																																																					}
																																																					Rows[7] ^= LS[69];
																																																					Columns[6] ^= LS[69];
																																																				}
																																																				Rows[7] ^= LS[65];
																																																				Columns[2] ^= LS[65];
																																																			}
																																																			Rows[7] ^= LS[63];
																																																			Columns[0] ^= LS[63];
																																																		}
																																																		Rows[7] ^= LS[67];
																																																		Columns[4] ^= LS[67];
																																																	}
																																																	Rows[4] ^= LS[41];
																																																}
																																																Rows[7] ^= LS[68];
																																																Columns[5] ^= LS[68];
																																															}
																																															Rows[4] ^= LS[39];
																																														}
																																														Rows[7] ^= LS[66];
																																														Columns[3] ^= LS[66];
																																													}
																																													Columns[8] ^= LS[62];
																																												}
																																												Rows[6] ^= LS[54];
																																												Columns[0] ^= LS[54];
																																											}
																																											Rows[6] ^= LS[61];
																																											Columns[7] ^= LS[61];
																																										}
																																										Rows[6] ^= LS[58];
																																										Columns[4] ^= LS[58];
																																									}
																																									Rows[6] ^= LS[55];
																																									Columns[1] ^= LS[55];
																																								}
																																								Rows[6] ^= LS[59];
																																								Columns[5] ^= LS[59];
																																							}
																																							Rows[6] ^= LS[57];
																																							Columns[3] ^= LS[57];
																																						}
																																						Columns[8] ^= LS[26];
																																					}
																																					Rows[2] ^= LS[18];
																																					Columns[0] ^= LS[18];
																																				}
																																				Rows[2] ^= LS[25];
																																				Columns[7] ^= LS[25];
																																			}
																																			Rows[2] ^= LS[22];
																																			Columns[4] ^= LS[22];
																																		}
																																		Rows[2] ^= LS[19];
																																		Columns[1] ^= LS[19];
																																	}
																																	Rows[2] ^= LS[23];
																																	Columns[5] ^= LS[23];
																																}
																																Rows[2] ^= LS[21];
																																Columns[3] ^= LS[21];
																															}
																															Columns[8] ^= LS[17];
																														}
																														Rows[1] ^= LS[9];
																														Columns[0] ^= LS[9];
																													}
																													Rows[1] ^= LS[13];
																													Columns[4] ^= LS[13];
																												}
																												Rows[1] ^= LS[15];
																												Columns[6] ^= LS[15];
																											}
																											Rows[1] ^= LS[14];
																											Columns[5] ^= LS[14];
																										}
																										Rows[1] ^= LS[12];
																										Columns[3] ^= LS[12];
																									}
																									Rows[1] ^= LS[11];
																									Columns[2] ^= LS[11];
																								}
																								Columns[7] ^= LS[79];
																							}
																							Rows[8] ^= LS[78];
																							Columns[6] ^= LS[78];
																						}
																						Rows[8] ^= LS[77];
																						Columns[5] ^= LS[77];
																					}
																					Rows[8] ^= LS[76];
																					Columns[4] ^= LS[76];
																				}
																				Rows[8] ^= LS[75];
																				Columns[3] ^= LS[75];
																			}
																			Rows[8] ^= LS[74];
																			Columns[2] ^= LS[74];
																		}
																		Rows[8] ^= LS[73];
																		Columns[1] ^= LS[73];
																	}
																	Rows[8] ^= LS[80];
																	Columns[8] ^= LS[80];

																}
															}
															Rows[5] ^= LS[50];
															Columns[5] ^= LS[50];
															MD ^= LS[50];
														}
														Rows[8] ^= LS[72];
														Columns[0] ^= LS[72];
													}
													Rows[5] ^= LS[48];
													Columns[3] ^= LS[48];
													AD ^= LS[48];
												}
												Rows[3] ^= LS[32];
												Columns[5] ^= LS[32];
												AD ^= LS[32];
											}
											Rows[3] ^= LS[30];
											Columns[3] ^= LS[30];
											MD ^= LS[30];
										}
										Rows[6] ^= LS[60];
										Columns[6] ^= LS[60];
										MD ^= LS[60];
									}
									Rows[6] ^= LS[56];
									Columns[2] ^= LS[56];
									AD ^= LS[56];
								}
								Rows[2] ^= LS[24];
								Columns[6] ^= LS[24];
								AD ^= LS[24];
							}
							Rows[2] ^= LS[20];
							Columns[2] ^= LS[20];
							MD ^= LS[20];
						}
						Rows[7] ^= LS[70];
						Columns[7] ^= LS[70];
						MD ^= LS[70];
					}
					Rows[7] ^= LS[64];
					Columns[1] ^= LS[64];
					AD ^= LS[64];
				}
				Rows[1] ^= LS[16];
				Columns[7] ^= LS[16];
				AD ^= LS[16];
			}
			Rows[1] ^= LS[10];
			Columns[1] ^= LS[10];
			MD ^= LS[10];
		}
		Rows[4] ^= LS[40];
		Columns[4] ^= LS[40];
		MD ^= LS[40];
		AD ^= LS[40];
	}
	TotalCnt += SquaresCnt*multiple;

	cout << "Processed " << processed << " workunits" << endl;
	return TotalCnt;
	//	return SquaresCnt;
}


uint64_t Generate_rc_X()
{
	double t_start = cpuTime();
	uint64_t SquaresCnt = 0;
	int32_t LS[N*N];
	int32_t CR[N*N];
	uint64_t multiple = 0;
	uint64_t TotalCnt = 0;
	uint64_t processed = 0;
	uint64_t k = 0;

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
	MD |= LS[0];
	AD |= LS[N - 1];

	//9 14 18 21 27 28 35 36 42 45 49 54 56 57 58 59 60 61 62 63
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19

	for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); CR[40] != 0; CR[40] = (CR[40] & (CR[40] - 1))) {
		LS[40] = (CR[40] & (-CR[40]));
		//{
		//LS[40] = WU[0];
		Rows[4] |= LS[40];
		Columns[4] |= LS[40];
		MD |= LS[40];
		AD |= LS[40];
		for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); CR[10] != 0; CR[10] = (CR[10] & (CR[10] - 1))) {
			LS[10] = (CR[10] & (-CR[10]));
			//{
			//LS[10] = WU[1];
			Rows[1] |= LS[10];
			Columns[1] |= LS[10];
			MD |= LS[10];
			for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); CR[16] != 0; CR[16] = (CR[16] & (CR[16] - 1))) {
				LS[16] = (CR[16] & (-CR[16]));
				//{
				//LS[16] = WU[2];
				Rows[1] |= LS[16];
				Columns[7] |= LS[16];
				AD |= LS[16];
				for (CR[64] = AllN ^ (Rows[7] | Columns[1] | AD); CR[64] != 0; CR[64] = (CR[64] & (CR[64] - 1))) {
					LS[64] = (CR[64] & (-CR[64]));
					//{
					//LS[64] = WU[3];
					Rows[7] |= LS[64];
					Columns[1] |= LS[64];
					AD |= LS[64];
					for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); CR[70] != 0; CR[70] = (CR[70] & (CR[70] - 1))) {
						LS[70] = (CR[70] & (-CR[70]));
						//{
						//LS[70] = WU[4];
						Rows[7] |= LS[70];
						Columns[7] |= LS[70];
						MD |= LS[70];
						for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); CR[20] != 0; CR[20] = (CR[20] & (CR[20] - 1))) {
							LS[20] = (CR[20] & (-CR[20]));
							//{
							//LS[20] = WU[5];
							Rows[2] |= LS[20];
							Columns[2] |= LS[20];
							MD |= LS[20];
							for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); CR[24] != 0; CR[24] = (CR[24] & (CR[24] - 1))) {
								LS[24] = (CR[24] & (-CR[24]));
								//{
								//	LS[24] = WU[6];
								Rows[2] |= LS[24];
								Columns[6] |= LS[24];
								AD |= LS[24];
								for (CR[56] = AllN ^ (Rows[6] | Columns[2] | AD); CR[56] != 0; CR[56] = (CR[56] & (CR[56] - 1))) {
									LS[56] = (CR[56] & (-CR[56]));
									//{
									//LS[56] = WU[7];
									Rows[6] |= LS[56];
									Columns[2] |= LS[56];
									AD |= LS[56];
									for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); CR[60] != 0; CR[60] = (CR[60] & (CR[60] - 1))) {
										LS[60] = (CR[60] & (-CR[60]));
										//{
										//	LS[60] = WU[8];
										Rows[6] |= LS[60];
										Columns[6] |= LS[60];
										MD |= LS[60];
										for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); CR[30] != 0; CR[30] = (CR[30] & (CR[30] - 1))) {
											LS[30] = (CR[30] & (-CR[30]));
											//{
											//LS[30] = WU[9];
											Rows[3] |= LS[30];
											Columns[3] |= LS[30];
											MD |= LS[30];

											for (CR[32] = AllN ^ (Rows[3] | Columns[5] | AD); CR[32] != 0; CR[32] = (CR[32] & (CR[32] - 1))) {
												LS[32] = (CR[32] & (-CR[32]));
												Rows[3] |= LS[32];
												Columns[5] |= LS[32];
												AD |= LS[32];
												for (CR[48] = AllN ^ (Rows[5] | Columns[3] | AD); CR[48] != 0; CR[48] = (CR[48] & (CR[48] - 1))) {
													LS[48] = (CR[48] & (-CR[48]));
													Rows[5] |= LS[48];
													Columns[3] |= LS[48];
													AD |= LS[48];
													CR[72] = AllN ^ (Rows[8] | Columns[0] | AD);
													if (CR[72] != 0) {
														LS[72] = (CR[72] & (-CR[72]));
														Rows[8] |= LS[72];
														Columns[0] |= LS[72];
														for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); CR[50] != 0; CR[50] = (CR[50] & (CR[50] - 1))) {
															LS[50] = (CR[50] & (-CR[50]));
															Rows[5] |= LS[50];
															Columns[5] |= LS[50];
															MD |= LS[50];
															CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
															if (CR[80] != 0) {
																LS[80] = (CR[80] & (-CR[80]));
																vector<int> LSM_X = { LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																	LS[10],LS[16],
																	LS[20],LS[24],
																	LS[30],LS[32],
																	LS[40],
																	LS[48],LS[50],
																	LS[56],LS[60],
																	LS[64],LS[70],
																	LS[72],LS[80]
																};

																k++;
																TotalCnt += SquaresCnt*multiple;
																cout << "Total count " << TotalCnt << endl;
																double t_cur = cpuTime();
																cout << "Average performance " << (double)TotalCnt / (t_cur - t_start) << endl;

																multiple = PROCESS_LSM_X(LSM_X);
																SquaresCnt = 0;
																if (multiple != 0) {
																	processed++;


																	Rows[8] |= LS[80];
																	Columns[8] |= LS[80];

																	/*print_LSM(LSM_X);
																	cout << "Flip vertical\n";
																	flip_vertical_X(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Normalize\n";
																	normalize(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Transform 2 {0,1,1,1}\n";
																	Transform2_x(LSM_X, { 0,1,1,1 });
																	print_LSM(LSM_X);

																	cout << "Normalize\n";
																	normalize(LSM_X);
																	print_LSM(LSM_X);

																	cout << "Transform 4 {3,1,2}\n";
																	Transform4_x(LSM_X, { 3,1,2 });
																	print_LSM(LSM_X);


																	getchar();
																	*/

																	for (CR[73] = AllN ^ (Rows[8] | Columns[1]); CR[73] != 0; CR[73] = (CR[73] & (CR[73] - 1))) {
																		LS[73] = (CR[73] & (-CR[73]));
																		Rows[8] |= LS[73];
																		Columns[1] |= LS[73];
																		for (CR[74] = AllN ^ (Rows[8] | Columns[2]); CR[74] != 0; CR[74] = (CR[74] & (CR[74] - 1))) {
																			LS[74] = (CR[74] & (-CR[74]));
																			Rows[8] |= LS[74];
																			Columns[2] |= LS[74];
																			for (CR[75] = AllN ^ (Rows[8] | Columns[3]); CR[75] != 0; CR[75] = (CR[75] & (CR[75] - 1))) {
																				LS[75] = (CR[75] & (-CR[75]));
																				Rows[8] |= LS[75];
																				Columns[3] |= LS[75];
																				for (CR[76] = AllN ^ (Rows[8] | Columns[4]); CR[76] != 0; CR[76] = (CR[76] & (CR[76] - 1))) {
																					LS[76] = (CR[76] & (-CR[76]));
																					Rows[8] |= LS[76];
																					Columns[4] |= LS[76];
																					for (CR[77] = AllN ^ (Rows[8] | Columns[5]); CR[77] != 0; CR[77] = (CR[77] & (CR[77] - 1))) {
																						LS[77] = (CR[77] & (-CR[77]));
																						Rows[8] |= LS[77];
																						Columns[5] |= LS[77];
																						for (CR[78] = AllN ^ (Rows[8] | Columns[6]); CR[78] != 0; CR[78] = (CR[78] & (CR[78] - 1))) {
																							LS[78] = (CR[78] & (-CR[78]));
																							Rows[8] |= LS[78];
																							Columns[6] |= LS[78];
																							CR[79] = AllN ^ (Rows[8] | Columns[7]);
																							if (CR[79] != 0) {
																								LS[79] = (CR[79] & (-CR[79]));

																								/*		vector<int> LSM_H = {LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																								LS[10],LS[16],
																								LS[20],LS[24],
																								LS[30],LS[32],
																								LS[40],
																								LS[48],LS[50],
																								LS[56],LS[60],
																								LS[64],LS[70],
																								LS[72],LS[73],LS[74],LS[75],LS[76],LS[77],LS[78],LS[79],LS[80]
																								};*/

																								/*print_LSM(LSM_H);
																								cout << "Flip vertical\n";
																								flip_vertical_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Flip horizontal\n";
																								flip_horizontal_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 2 {0,1,1,1}\n";
																								Transform2_h(LSM_H, { 1,1,1,1 });
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 4 {3,1,2}\n";
																								Transform4_h(LSM_H, { 3,1,2 });
																								print_LSM(LSM_H);
																								getchar();
																								*/

																								Columns[7] |= LS[79];
																								for (CR[11] = AllN ^ (Rows[1] | Columns[2]); CR[11] != 0; CR[11] = (CR[11] & (CR[11] - 1))) {
																									LS[11] = (CR[11] & (-CR[11]));
																									Rows[1] |= LS[11];
																									Columns[2] |= LS[11];
																									for (CR[12] = AllN ^ (Rows[1] | Columns[3]); CR[12] != 0; CR[12] = (CR[12] & (CR[12] - 1))) {
																										LS[12] = (CR[12] & (-CR[12]));
																										Rows[1] |= LS[12];
																										Columns[3] |= LS[12];
																										for (CR[14] = AllN ^ (Rows[1] | Columns[5]); CR[14] != 0; CR[14] = (CR[14] & (CR[14] - 1))) {
																											LS[14] = (CR[14] & (-CR[14]));
																											Rows[1] |= LS[14];
																											Columns[5] |= LS[14];
																											for (CR[15] = AllN ^ (Rows[1] | Columns[6]); CR[15] != 0; CR[15] = (CR[15] & (CR[15] - 1))) {
																												LS[15] = (CR[15] & (-CR[15]));
																												Rows[1] |= LS[15];
																												Columns[6] |= LS[15];
																												for (CR[13] = AllN ^ (Rows[1] | Columns[4]); CR[13] != 0; CR[13] = (CR[13] & (CR[13] - 1))) {
																													LS[13] = (CR[13] & (-CR[13]));
																													Rows[1] |= LS[13];
																													Columns[4] |= LS[13];
																													for (CR[9] = AllN ^ (Rows[1] | Columns[0]); CR[9] != 0; CR[9] = (CR[9] & (CR[9] - 1))) {
																														LS[9] = (CR[9] & (-CR[9]));
																														Rows[1] |= LS[9];
																														Columns[0] |= LS[9];
																														CR[17] = AllN ^ (Rows[1] | Columns[8]);
																														if (CR[17] != 0) {
																															LS[17] = (CR[17] & (-CR[17]));
																															Columns[8] |= LS[17];
																															for (CR[21] = AllN ^ (Rows[2] | Columns[3]); CR[21] != 0; CR[21] = (CR[21] & (CR[21] - 1))) {
																																LS[21] = (CR[21] & (-CR[21]));
																																Rows[2] |= LS[21];
																																Columns[3] |= LS[21];
																																for (CR[23] = AllN ^ (Rows[2] | Columns[5]); CR[23] != 0; CR[23] = (CR[23] & (CR[23] - 1))) {
																																	LS[23] = (CR[23] & (-CR[23]));
																																	Rows[2] |= LS[23];
																																	Columns[5] |= LS[23];
																																	for (CR[19] = AllN ^ (Rows[2] | Columns[1]); CR[19] != 0; CR[19] = (CR[19] & (CR[19] - 1))) {
																																		LS[19] = (CR[19] & (-CR[19]));
																																		Rows[2] |= LS[19];
																																		Columns[1] |= LS[19];
																																		for (CR[22] = AllN ^ (Rows[2] | Columns[4]); CR[22] != 0; CR[22] = (CR[22] & (CR[22] - 1))) {
																																			LS[22] = (CR[22] & (-CR[22]));
																																			Rows[2] |= LS[22];
																																			Columns[4] |= LS[22];
																																			for (CR[25] = AllN ^ (Rows[2] | Columns[7]); CR[25] != 0; CR[25] = (CR[25] & (CR[25] - 1))) {
																																				LS[25] = (CR[25] & (-CR[25]));
																																				Rows[2] |= LS[25];
																																				Columns[7] |= LS[25];
																																				for (CR[18] = AllN ^ (Rows[2] | Columns[0]); CR[18] != 0; CR[18] = (CR[18] & (CR[18] - 1))) {
																																					LS[18] = (CR[18] & (-CR[18]));
																																					Rows[2] |= LS[18];
																																					Columns[0] |= LS[18];
																																					CR[26] = AllN ^ (Rows[2] | Columns[8]);
																																					if (CR[26] != 0) {
																																						LS[26] = (CR[26] & (-CR[26]));
																																						Columns[8] |= LS[26];
																																						for (CR[57] = AllN ^ (Rows[6] | Columns[3]); CR[57] != 0; CR[57] = (CR[57] & (CR[57] - 1))) {
																																							LS[57] = (CR[57] & (-CR[57]));
																																							Rows[6] |= LS[57];
																																							Columns[3] |= LS[57];
																																							for (CR[59] = AllN ^ (Rows[6] | Columns[5]); CR[59] != 0; CR[59] = (CR[59] & (CR[59] - 1))) {
																																								LS[59] = (CR[59] & (-CR[59]));
																																								Rows[6] |= LS[59];
																																								Columns[5] |= LS[59];
																																								for (CR[55] = AllN ^ (Rows[6] | Columns[1]); CR[55] != 0; CR[55] = (CR[55] & (CR[55] - 1))) {
																																									LS[55] = (CR[55] & (-CR[55]));
																																									Rows[6] |= LS[55];
																																									Columns[1] |= LS[55];
																																									for (CR[58] = AllN ^ (Rows[6] | Columns[4]); CR[58] != 0; CR[58] = (CR[58] & (CR[58] - 1))) {
																																										LS[58] = (CR[58] & (-CR[58]));
																																										Rows[6] |= LS[58];
																																										Columns[4] |= LS[58];
																																										for (CR[61] = AllN ^ (Rows[6] | Columns[7]); CR[61] != 0; CR[61] = (CR[61] & (CR[61] - 1))) {
																																											LS[61] = (CR[61] & (-CR[61]));
																																											Rows[6] |= LS[61];
																																											Columns[7] |= LS[61];
																																											for (CR[54] = AllN ^ (Rows[6] | Columns[0]); CR[54] != 0; CR[54] = (CR[54] & (CR[54] - 1))) {
																																												LS[54] = (CR[54] & (-CR[54]));
																																												Rows[6] |= LS[54];
																																												Columns[0] |= LS[54];
																																												CR[62] = AllN ^ (Rows[6] | Columns[8]);
																																												if (CR[62] != 0) {
																																													LS[62] = (CR[62] & (-CR[62]));
																																													Columns[8] |= LS[62];
																																													for (CR[66] = AllN ^ (Rows[7] | Columns[3]); CR[66] != 0; CR[66] = (CR[66] & (CR[66] - 1))) {
																																														LS[66] = (CR[66] & (-CR[66]));
																																														Rows[7] |= LS[66];
																																														Columns[3] |= LS[66];
																																														CR[39] = AllN ^ (Rows[4] | Columns[3]);
																																														if (CR[39] != 0) {
																																															LS[39] = (CR[39] & (-CR[39]));
																																															Rows[4] |= LS[39];
																																															for (CR[68] = AllN ^ (Rows[7] | Columns[5]); CR[68] != 0; CR[68] = (CR[68] & (CR[68] - 1))) {
																																																LS[68] = (CR[68] & (-CR[68]));
																																																Rows[7] |= LS[68];
																																																Columns[5] |= LS[68];
																																																CR[41] = AllN ^ (Rows[4] | Columns[5]);
																																																if (CR[41] != 0) {
																																																	LS[41] = (CR[41] & (-CR[41]));
																																																	Rows[4] |= LS[41];
																																																	for (CR[67] = AllN ^ (Rows[7] | Columns[4]); CR[67] != 0; CR[67] = (CR[67] & (CR[67] - 1))) {
																																																		LS[67] = (CR[67] & (-CR[67]));
																																																		Rows[7] |= LS[67];
																																																		Columns[4] |= LS[67];
																																																		for (CR[63] = AllN ^ (Rows[7] | Columns[0]); CR[63] != 0; CR[63] = (CR[63] & (CR[63] - 1))) {
																																																			LS[63] = (CR[63] & (-CR[63]));
																																																			Rows[7] |= LS[63];
																																																			Columns[0] |= LS[63];
																																																			for (CR[65] = AllN ^ (Rows[7] | Columns[2]); CR[65] != 0; CR[65] = (CR[65] & (CR[65] - 1))) {
																																																				LS[65] = (CR[65] & (-CR[65]));
																																																				Rows[7] |= LS[65];
																																																				Columns[2] |= LS[65];
																																																				for (CR[69] = AllN ^ (Rows[7] | Columns[6]); CR[69] != 0; CR[69] = (CR[69] & (CR[69] - 1))) {
																																																					LS[69] = (CR[69] & (-CR[69]));
																																																					Rows[7] |= LS[69];
																																																					Columns[6] |= LS[69];
																																																					CR[71] = AllN ^ (Rows[7] | Columns[8]);
																																																					if (CR[71] != 0) {
																																																						LS[71] = (CR[71] & (-CR[71]));
																																																						Columns[8] |= LS[71];
																																																						for (CR[31] = AllN ^ (Rows[3] | Columns[4]); CR[31] != 0; CR[31] = (CR[31] & (CR[31] - 1))) {
																																																							LS[31] = (CR[31] & (-CR[31]));
																																																							Rows[3] |= LS[31];
																																																							Columns[4] |= LS[31];
																																																							CR[49] = AllN ^ (Rows[5] | Columns[4]);
																																																							if (CR[49] != 0) {
																																																								LS[49] = (CR[49] & (-CR[49]));
																																																								Rows[5] |= LS[49];
																																																								for (CR[27] = AllN ^ (Rows[3] | Columns[0]); CR[27] != 0; CR[27] = (CR[27] & (CR[27] - 1))) {
																																																									LS[27] = (CR[27] & (-CR[27]));
																																																									Rows[3] |= LS[27];
																																																									Columns[0] |= LS[27];
																																																									for (CR[28] = AllN ^ (Rows[3] | Columns[1]); CR[28] != 0; CR[28] = (CR[28] & (CR[28] - 1))) {
																																																										LS[28] = (CR[28] & (-CR[28]));
																																																										Rows[3] |= LS[28];
																																																										Columns[1] |= LS[28];
																																																										for (CR[29] = AllN ^ (Rows[3] | Columns[2]); CR[29] != 0; CR[29] = (CR[29] & (CR[29] - 1))) {
																																																											LS[29] = (CR[29] & (-CR[29]));
																																																											Rows[3] |= LS[29];
																																																											Columns[2] |= LS[29];
																																																											for (CR[33] = AllN ^ (Rows[3] | Columns[6]); CR[33] != 0; CR[33] = (CR[33] & (CR[33] - 1))) {
																																																												LS[33] = (CR[33] & (-CR[33]));
																																																												Rows[3] |= LS[33];
																																																												Columns[6] |= LS[33];
																																																												for (CR[34] = AllN ^ (Rows[3] | Columns[7]); CR[34] != 0; CR[34] = (CR[34] & (CR[34] - 1))) {
																																																													LS[34] = (CR[34] & (-CR[34]));
																																																													Rows[3] |= LS[34];
																																																													Columns[7] |= LS[34];
																																																													CR[35] = AllN ^ (Rows[3] | Columns[8]);
																																																													if (CR[35] != 0) {
																																																														LS[35] = (CR[35] & (-CR[35]));
																																																														Columns[8] |= LS[35];
																																																														for (CR[36] = AllN ^ (Rows[4] | Columns[0]); CR[36] != 0; CR[36] = (CR[36] & (CR[36] - 1))) {
																																																															LS[36] = (CR[36] & (-CR[36]));
																																																															Rows[4] |= LS[36];
																																																															Columns[0] |= LS[36];
																																																															CR[45] = AllN ^ (Rows[5] | Columns[0]);
																																																															if (CR[45] != 0) {
																																																																LS[45] = (CR[45] & (-CR[45]));
																																																																Rows[5] |= LS[45];
																																																																for (CR[37] = AllN ^ (Rows[4] | Columns[1]); CR[37] != 0; CR[37] = (CR[37] & (CR[37] - 1))) {
																																																																	LS[37] = (CR[37] & (-CR[37]));
																																																																	Rows[4] |= LS[37];
																																																																	Columns[1] |= LS[37];
																																																																	CR[46] = AllN ^ (Rows[5] | Columns[1]);
																																																																	if (CR[46] != 0) {
																																																																		LS[46] = (CR[46] & (-CR[46]));
																																																																		Rows[5] |= LS[46];
																																																																		for (CR[38] = AllN ^ (Rows[4] | Columns[2]); CR[38] != 0; CR[38] = (CR[38] & (CR[38] - 1))) {
																																																																			LS[38] = (CR[38] & (-CR[38]));
																																																																			Rows[4] |= LS[38];
																																																																			Columns[2] |= LS[38];
																																																																			CR[47] = AllN ^ (Rows[5] | Columns[2]);
																																																																			if (CR[47] != 0) {
																																																																				LS[47] = (CR[47] & (-CR[47]));
																																																																				Rows[5] |= LS[47];
																																																																				for (CR[42] = AllN ^ (Rows[4] | Columns[6]); CR[42] != 0; CR[42] = (CR[42] & (CR[42] - 1))) {
																																																																					LS[42] = (CR[42] & (-CR[42]));
																																																																					Rows[4] |= LS[42];
																																																																					Columns[6] |= LS[42];
																																																																					CR[51] = AllN ^ (Rows[5] | Columns[6]);
																																																																					if (CR[51] != 0) {
																																																																						LS[51] = (CR[51] & (-CR[51]));
																																																																						Rows[5] |= LS[51];
																																																																						for (CR[43] = AllN ^ (Rows[4] | Columns[7]); CR[43] != 0; CR[43] = (CR[43] & (CR[43] - 1))) {
																																																																							LS[43] = (CR[43] & (-CR[43]));
																																																																							Rows[4] |= LS[43];
																																																																							Columns[7] |= LS[43];
																																																																							CR[44] = AllN ^ (Rows[4] | Columns[8]);
																																																																							if (CR[44] != 0) {
																																																																								LS[44] = (CR[44] & (-CR[44]));
																																																																								Columns[8] |= LS[44];
																																																																								CR[52] = AllN ^ (Rows[5] | Columns[7]);
																																																																								if (CR[52] != 0) {
																																																																									LS[52] = (CR[52] & (-CR[52]));
																																																																									Rows[5] |= LS[52];
																																																																									CR[53] = AllN ^ (Rows[5] | Columns[8]);
																																																																									if (CR[53] != 0) {
																																																																										LS[53] = (CR[53] & (-CR[53]));
																																																																										SquaresCnt++;
																																																																										if (SquaresCnt % 10000000 == 0) {
																																																																											cout << "\rSquares found: " << SquaresCnt;// << endl;

																																																																										}
																																																																									}
																																																																									Rows[5] ^= LS[52];
																																																																								}
																																																																								Columns[8] ^= LS[44];
																																																																							}
																																																																							Rows[4] ^= LS[43];
																																																																							Columns[7] ^= LS[43];
																																																																						}
																																																																						Rows[5] ^= LS[51];
																																																																					}
																																																																					Rows[4] ^= LS[42];
																																																																					Columns[6] ^= LS[42];
																																																																				}
																																																																				Rows[5] ^= LS[47];
																																																																			}
																																																																			Rows[4] ^= LS[38];
																																																																			Columns[2] ^= LS[38];
																																																																		}
																																																																		Rows[5] ^= LS[46];
																																																																	}
																																																																	Rows[4] ^= LS[37];
																																																																	Columns[1] ^= LS[37];
																																																																}
																																																																Rows[5] ^= LS[45];
																																																															}
																																																															Rows[4] ^= LS[36];
																																																															Columns[0] ^= LS[36];
																																																														}
																																																														Columns[8] ^= LS[35];
																																																													}
																																																													Rows[3] ^= LS[34];
																																																													Columns[7] ^= LS[34];
																																																												}
																																																												Rows[3] ^= LS[33];
																																																												Columns[6] ^= LS[33];
																																																											}
																																																											Rows[3] ^= LS[29];
																																																											Columns[2] ^= LS[29];
																																																										}
																																																										Rows[3] ^= LS[28];
																																																										Columns[1] ^= LS[28];
																																																									}
																																																									Rows[3] ^= LS[27];
																																																									Columns[0] ^= LS[27];
																																																								}
																																																								Rows[5] ^= LS[49];
																																																							}
																																																							Rows[3] ^= LS[31];
																																																							Columns[4] ^= LS[31];
																																																						}
																																																						Columns[8] ^= LS[71];
																																																					}
																																																					Rows[7] ^= LS[69];
																																																					Columns[6] ^= LS[69];
																																																				}
																																																				Rows[7] ^= LS[65];
																																																				Columns[2] ^= LS[65];
																																																			}
																																																			Rows[7] ^= LS[63];
																																																			Columns[0] ^= LS[63];
																																																		}
																																																		Rows[7] ^= LS[67];
																																																		Columns[4] ^= LS[67];
																																																	}
																																																	Rows[4] ^= LS[41];
																																																}
																																																Rows[7] ^= LS[68];
																																																Columns[5] ^= LS[68];
																																															}
																																															Rows[4] ^= LS[39];
																																														}
																																														Rows[7] ^= LS[66];
																																														Columns[3] ^= LS[66];
																																													}
																																													Columns[8] ^= LS[62];
																																												}
																																												Rows[6] ^= LS[54];
																																												Columns[0] ^= LS[54];
																																											}
																																											Rows[6] ^= LS[61];
																																											Columns[7] ^= LS[61];
																																										}
																																										Rows[6] ^= LS[58];
																																										Columns[4] ^= LS[58];
																																									}
																																									Rows[6] ^= LS[55];
																																									Columns[1] ^= LS[55];
																																								}
																																								Rows[6] ^= LS[59];
																																								Columns[5] ^= LS[59];
																																							}
																																							Rows[6] ^= LS[57];
																																							Columns[3] ^= LS[57];
																																						}
																																						Columns[8] ^= LS[26];
																																					}
																																					Rows[2] ^= LS[18];
																																					Columns[0] ^= LS[18];
																																				}
																																				Rows[2] ^= LS[25];
																																				Columns[7] ^= LS[25];
																																			}
																																			Rows[2] ^= LS[22];
																																			Columns[4] ^= LS[22];
																																		}
																																		Rows[2] ^= LS[19];
																																		Columns[1] ^= LS[19];
																																	}
																																	Rows[2] ^= LS[23];
																																	Columns[5] ^= LS[23];
																																}
																																Rows[2] ^= LS[21];
																																Columns[3] ^= LS[21];
																															}
																															Columns[8] ^= LS[17];
																														}
																														Rows[1] ^= LS[9];
																														Columns[0] ^= LS[9];
																													}
																													Rows[1] ^= LS[13];
																													Columns[4] ^= LS[13];
																												}
																												Rows[1] ^= LS[15];
																												Columns[6] ^= LS[15];
																											}
																											Rows[1] ^= LS[14];
																											Columns[5] ^= LS[14];
																										}
																										Rows[1] ^= LS[12];
																										Columns[3] ^= LS[12];
																									}
																									Rows[1] ^= LS[11];
																									Columns[2] ^= LS[11];
																								}
																								Columns[7] ^= LS[79];
																							}
																							Rows[8] ^= LS[78];
																							Columns[6] ^= LS[78];
																						}
																						Rows[8] ^= LS[77];
																						Columns[5] ^= LS[77];
																					}
																					Rows[8] ^= LS[76];
																					Columns[4] ^= LS[76];
																				}
																				Rows[8] ^= LS[75];
																				Columns[3] ^= LS[75];
																			}
																			Rows[8] ^= LS[74];
																			Columns[2] ^= LS[74];
																		}
																		Rows[8] ^= LS[73];
																		Columns[1] ^= LS[73];
																	}
																	Rows[8] ^= LS[80];
																	Columns[8] ^= LS[80];

																}
															}
															Rows[5] ^= LS[50];
															Columns[5] ^= LS[50];
															MD ^= LS[50];
														}
														Rows[8] ^= LS[72];
														Columns[0] ^= LS[72];
													}
													Rows[5] ^= LS[48];
													Columns[3] ^= LS[48];
													AD ^= LS[48];
												}
												Rows[3] ^= LS[32];
												Columns[5] ^= LS[32];
												AD ^= LS[32];
											}
											Rows[3] ^= LS[30];
											Columns[3] ^= LS[30];
											MD ^= LS[30];
										}
										Rows[6] ^= LS[60];
										Columns[6] ^= LS[60];
										MD ^= LS[60];
									}
									Rows[6] ^= LS[56];
									Columns[2] ^= LS[56];
									AD ^= LS[56];
								}
								Rows[2] ^= LS[24];
								Columns[6] ^= LS[24];
								AD ^= LS[24];
							}
							Rows[2] ^= LS[20];
							Columns[2] ^= LS[20];
							MD ^= LS[20];
						}
						Rows[7] ^= LS[70];
						Columns[7] ^= LS[70];
						MD ^= LS[70];
					}
					Rows[7] ^= LS[64];
					Columns[1] ^= LS[64];
					AD ^= LS[64];
				}
				Rows[1] ^= LS[16];
				Columns[7] ^= LS[16];
				AD ^= LS[16];
			}
			Rows[1] ^= LS[10];
			Columns[1] ^= LS[10];
			MD ^= LS[10];
		}
		Rows[4] ^= LS[40];
		Columns[4] ^= LS[40];
		MD ^= LS[40];
		AD ^= LS[40];
	}
	TotalCnt += SquaresCnt*multiple;

	cout << "Processed " << processed << " workunits" << endl;
	return TotalCnt;
	//	return SquaresCnt;
}



uint64_t Generate_rc_H(vector<int> WU)
{
	cur_cnt++;
	double t_start = cpuTime();
	uint64_t SquaresCnt = 0;
	int32_t LS[N*N];
	int32_t CR[N*N];
	uint64_t multiple = 0;
	uint64_t TotalCnt = 0;
	uint64_t processed = 0;
	uint64_t k = 0;

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
	MD |= LS[0];
	AD |= LS[N - 1];

	//9 14 18 21 27 28 35 36 42 45 49 54 56 57 58 59 60 61 62 63
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19

	//for (CR[40] = AllN ^ (Rows[4] | Columns[4] | MD | AD); CR[40] != 0; CR[40] = (CR[40] & (CR[40] - 1))) {
		//LS[40] = (CR[40] & (-CR[40]));
		{
		LS[40] = WU[0];
		Rows[4] |= LS[40];
		Columns[4] |= LS[40];
		MD |= LS[40];
		AD |= LS[40];
		//for (CR[10] = AllN ^ (Rows[1] | Columns[1] | MD); CR[10] != 0; CR[10] = (CR[10] & (CR[10] - 1))) {
		//	LS[10] = (CR[10] & (-CR[10]));
			{
			LS[10] = WU[1];
			Rows[1] |= LS[10];
			Columns[1] |= LS[10];
			MD |= LS[10];
			//for (CR[16] = AllN ^ (Rows[1] | Columns[7] | AD); CR[16] != 0; CR[16] = (CR[16] & (CR[16] - 1))) {
			//LS[16] = (CR[16] & (-CR[16]));
				{
				LS[16] = WU[2];
				Rows[1] |= LS[16];
				Columns[7] |= LS[16];
				AD |= LS[16];
				//for (CR[64] = AllN ^ (Rows[7] | Columns[1] | AD); CR[64] != 0; CR[64] = (CR[64] & (CR[64] - 1))) {
				//	LS[64] = (CR[64] & (-CR[64]));
					{
					LS[64] = WU[3];
					Rows[7] |= LS[64];
					Columns[1] |= LS[64];
					AD |= LS[64];
					//for (CR[70] = AllN ^ (Rows[7] | Columns[7] | MD); CR[70] != 0; CR[70] = (CR[70] & (CR[70] - 1))) {
						//LS[70] = (CR[70] & (-CR[70]));
						{
						LS[70] = WU[4];
						Rows[7] |= LS[70];
						Columns[7] |= LS[70];
						MD |= LS[70];
						//for (CR[20] = AllN ^ (Rows[2] | Columns[2] | MD); CR[20] != 0; CR[20] = (CR[20] & (CR[20] - 1))) {
							//LS[20] = (CR[20] & (-CR[20]));
							{
							LS[20] = WU[5];
							Rows[2] |= LS[20];
							Columns[2] |= LS[20];
							MD |= LS[20];
							//for (CR[24] = AllN ^ (Rows[2] | Columns[6] | AD); CR[24] != 0; CR[24] = (CR[24] & (CR[24] - 1))) {
							//	LS[24] = (CR[24] & (-CR[24]));
								{
									LS[24] = WU[6];
								Rows[2] |= LS[24];
								Columns[6] |= LS[24];
								AD |= LS[24];
								//for (CR[56] = AllN ^ (Rows[6] | Columns[2] | AD); CR[56] != 0; CR[56] = (CR[56] & (CR[56] - 1))) {
									//LS[56] = (CR[56] & (-CR[56]));
									{
									LS[56] = WU[7];
									Rows[6] |= LS[56];
									Columns[2] |= LS[56];
									AD |= LS[56];
									//for (CR[60] = AllN ^ (Rows[6] | Columns[6] | MD); CR[60] != 0; CR[60] = (CR[60] & (CR[60] - 1))) {
									//	LS[60] = (CR[60] & (-CR[60]));
										{
											LS[60] = WU[8];
										Rows[6] |= LS[60];
										Columns[6] |= LS[60];
										MD |= LS[60];
										//for (CR[30] = AllN ^ (Rows[3] | Columns[3] | MD); CR[30] != 0; CR[30] = (CR[30] & (CR[30] - 1))) {
											//LS[30] = (CR[30] & (-CR[30]));
											{
											LS[30] = WU[9];
											Rows[3] |= LS[30];
											Columns[3] |= LS[30];
											MD |= LS[30];

											for (CR[32] = AllN ^ (Rows[3] | Columns[5] | AD); CR[32] != 0; CR[32] = (CR[32] & (CR[32] - 1))) {
												LS[32] = (CR[32] & (-CR[32]));
												Rows[3] |= LS[32];
												Columns[5] |= LS[32];
												AD |= LS[32];
												for (CR[48] = AllN ^ (Rows[5] | Columns[3] | AD); CR[48] != 0; CR[48] = (CR[48] & (CR[48] - 1))) {
													LS[48] = (CR[48] & (-CR[48]));
													Rows[5] |= LS[48];
													Columns[3] |= LS[48];
													AD |= LS[48];
													CR[72] = AllN ^ (Rows[8] | Columns[0] | AD);
													if (CR[72] != 0) {
														LS[72] = (CR[72] & (-CR[72]));
														Rows[8] |= LS[72];
														Columns[0] |= LS[72];
														for (CR[50] = AllN ^ (Rows[5] | Columns[5] | MD); CR[50] != 0; CR[50] = (CR[50] & (CR[50] - 1))) {
															LS[50] = (CR[50] & (-CR[50]));
															Rows[5] |= LS[50];
															Columns[5] |= LS[50];
															MD |= LS[50];
															CR[80] = AllN ^ (Rows[8] | Columns[8] | MD);
															if (CR[80] != 0) {
																LS[80] = (CR[80] & (-CR[80]));
																/*	vector<int> LSM_X = { LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																LS[10],LS[16],
																LS[20],LS[24],
																LS[30],LS[32],
																LS[40],
																LS[48],LS[50],
																LS[56],LS[60],
																LS[64],LS[70],
																LS[72],LS[80]
																};

																k++;
																TotalCnt += SquaresCnt*multiple;
																multiple = PROCESS_LSM_X(LSM_X);
																SquaresCnt = 0;
																if (multiple != 0) {
																processed++;
																*/

																Rows[8] |= LS[80];
																Columns[8] |= LS[80];

																/*print_LSM(LSM_X);
																cout << "Flip vertical\n";
																flip_vertical_X(LSM_X);
																print_LSM(LSM_X);

																cout << "Normalize\n";
																normalize(LSM_X);
																print_LSM(LSM_X);

																cout << "Transform 2 {0,1,1,1}\n";
																Transform2_x(LSM_X, { 0,1,1,1 });
																print_LSM(LSM_X);

																cout << "Normalize\n";
																normalize(LSM_X);
																print_LSM(LSM_X);

																cout << "Transform 4 {3,1,2}\n";
																Transform4_x(LSM_X, { 3,1,2 });
																print_LSM(LSM_X);


																getchar();
																*/

																for (CR[73] = AllN ^ (Rows[8] | Columns[1]); CR[73] != 0; CR[73] = (CR[73] & (CR[73] - 1))) {
																	LS[73] = (CR[73] & (-CR[73]));
																	Rows[8] |= LS[73];
																	Columns[1] |= LS[73];
																	for (CR[74] = AllN ^ (Rows[8] | Columns[2]); CR[74] != 0; CR[74] = (CR[74] & (CR[74] - 1))) {
																		LS[74] = (CR[74] & (-CR[74]));
																		Rows[8] |= LS[74];
																		Columns[2] |= LS[74];
																		for (CR[75] = AllN ^ (Rows[8] | Columns[3]); CR[75] != 0; CR[75] = (CR[75] & (CR[75] - 1))) {
																			LS[75] = (CR[75] & (-CR[75]));
																			Rows[8] |= LS[75];
																			Columns[3] |= LS[75];
																			for (CR[76] = AllN ^ (Rows[8] | Columns[4]); CR[76] != 0; CR[76] = (CR[76] & (CR[76] - 1))) {
																				LS[76] = (CR[76] & (-CR[76]));
																				Rows[8] |= LS[76];
																				Columns[4] |= LS[76];
																				for (CR[77] = AllN ^ (Rows[8] | Columns[5]); CR[77] != 0; CR[77] = (CR[77] & (CR[77] - 1))) {
																					LS[77] = (CR[77] & (-CR[77]));
																					Rows[8] |= LS[77];
																					Columns[5] |= LS[77];
																					for (CR[78] = AllN ^ (Rows[8] | Columns[6]); CR[78] != 0; CR[78] = (CR[78] & (CR[78] - 1))) {
																						LS[78] = (CR[78] & (-CR[78]));
																						Rows[8] |= LS[78];
																						Columns[6] |= LS[78];
																						CR[79] = AllN ^ (Rows[8] | Columns[7]);
																						if (CR[79] != 0) {
																							LS[79] = (CR[79] & (-CR[79]));

																							vector<int> LSM_H = { LS[0], LS[1],LS[2],LS[3],LS[4],LS[5],LS[6],LS[7],LS[8],
																								LS[10],LS[16],
																								LS[20],LS[24],
																								LS[30],LS[32],
																								LS[40],
																								LS[48],LS[50],
																								LS[56],LS[60],
																								LS[64],LS[70],
																								LS[72],LS[73],LS[74],LS[75],LS[76],LS[77],LS[78],LS[79],LS[80]
																							};



																							k++;
																							TotalCnt += SquaresCnt*multiple;
																							//cout << "Total count " << TotalCnt << endl;
																							//double t_cur = cpuTime();
																							//cout << "Average performance " << (double)TotalCnt / (t_cur - t_start) << endl;

																							multiple = PROCESS_LSM_H(LSM_H);
																							SquaresCnt = 0;
																							if (multiple != 0) {
																								processed++;


																								/*print_LSM(LSM_H);
																								cout << "Flip vertical\n";
																								flip_vertical_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Flip horizontal\n";
																								flip_horizontal_H(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 2 {0,1,1,1}\n";
																								Transform2_h(LSM_H, { 1,1,1,1 });
																								print_LSM(LSM_H);

																								cout << "Normalize\n";
																								normalize(LSM_H);
																								print_LSM(LSM_H);

																								cout << "Transform 4 {3,1,2}\n";
																								Transform4_h(LSM_H, { 3,1,2 });
																								print_LSM(LSM_H);
																								getchar();
																								*/

																								Columns[7] |= LS[79];
																								for (CR[11] = AllN ^ (Rows[1] | Columns[2]); CR[11] != 0; CR[11] = (CR[11] & (CR[11] - 1))) {
																									LS[11] = (CR[11] & (-CR[11]));
																									Rows[1] |= LS[11];
																									Columns[2] |= LS[11];
																									for (CR[12] = AllN ^ (Rows[1] | Columns[3]); CR[12] != 0; CR[12] = (CR[12] & (CR[12] - 1))) {
																										LS[12] = (CR[12] & (-CR[12]));
																										Rows[1] |= LS[12];
																										Columns[3] |= LS[12];
																										for (CR[14] = AllN ^ (Rows[1] | Columns[5]); CR[14] != 0; CR[14] = (CR[14] & (CR[14] - 1))) {
																											LS[14] = (CR[14] & (-CR[14]));
																											Rows[1] |= LS[14];
																											Columns[5] |= LS[14];
																											for (CR[15] = AllN ^ (Rows[1] | Columns[6]); CR[15] != 0; CR[15] = (CR[15] & (CR[15] - 1))) {
																												LS[15] = (CR[15] & (-CR[15]));
																												Rows[1] |= LS[15];
																												Columns[6] |= LS[15];
																												for (CR[13] = AllN ^ (Rows[1] | Columns[4]); CR[13] != 0; CR[13] = (CR[13] & (CR[13] - 1))) {
																													LS[13] = (CR[13] & (-CR[13]));
																													Rows[1] |= LS[13];
																													Columns[4] |= LS[13];
																													for (CR[9] = AllN ^ (Rows[1] | Columns[0]); CR[9] != 0; CR[9] = (CR[9] & (CR[9] - 1))) {
																														LS[9] = (CR[9] & (-CR[9]));
																														Rows[1] |= LS[9];
																														Columns[0] |= LS[9];
																														CR[17] = AllN ^ (Rows[1] | Columns[8]);
																														if (CR[17] != 0) {
																															LS[17] = (CR[17] & (-CR[17]));
																															Columns[8] |= LS[17];
																															for (CR[21] = AllN ^ (Rows[2] | Columns[3]); CR[21] != 0; CR[21] = (CR[21] & (CR[21] - 1))) {
																																LS[21] = (CR[21] & (-CR[21]));
																																Rows[2] |= LS[21];
																																Columns[3] |= LS[21];
																																for (CR[23] = AllN ^ (Rows[2] | Columns[5]); CR[23] != 0; CR[23] = (CR[23] & (CR[23] - 1))) {
																																	LS[23] = (CR[23] & (-CR[23]));
																																	Rows[2] |= LS[23];
																																	Columns[5] |= LS[23];
																																	for (CR[19] = AllN ^ (Rows[2] | Columns[1]); CR[19] != 0; CR[19] = (CR[19] & (CR[19] - 1))) {
																																		LS[19] = (CR[19] & (-CR[19]));
																																		Rows[2] |= LS[19];
																																		Columns[1] |= LS[19];
																																		for (CR[22] = AllN ^ (Rows[2] | Columns[4]); CR[22] != 0; CR[22] = (CR[22] & (CR[22] - 1))) {
																																			LS[22] = (CR[22] & (-CR[22]));
																																			Rows[2] |= LS[22];
																																			Columns[4] |= LS[22];
																																			for (CR[25] = AllN ^ (Rows[2] | Columns[7]); CR[25] != 0; CR[25] = (CR[25] & (CR[25] - 1))) {
																																				LS[25] = (CR[25] & (-CR[25]));
																																				Rows[2] |= LS[25];
																																				Columns[7] |= LS[25];
																																				for (CR[18] = AllN ^ (Rows[2] | Columns[0]); CR[18] != 0; CR[18] = (CR[18] & (CR[18] - 1))) {
																																					LS[18] = (CR[18] & (-CR[18]));
																																					Rows[2] |= LS[18];
																																					Columns[0] |= LS[18];
																																					CR[26] = AllN ^ (Rows[2] | Columns[8]);
																																					if (CR[26] != 0) {
																																						LS[26] = (CR[26] & (-CR[26]));
																																						Columns[8] |= LS[26];
																																						for (CR[57] = AllN ^ (Rows[6] | Columns[3]); CR[57] != 0; CR[57] = (CR[57] & (CR[57] - 1))) {
																																							LS[57] = (CR[57] & (-CR[57]));
																																							Rows[6] |= LS[57];
																																							Columns[3] |= LS[57];
																																							for (CR[59] = AllN ^ (Rows[6] | Columns[5]); CR[59] != 0; CR[59] = (CR[59] & (CR[59] - 1))) {
																																								LS[59] = (CR[59] & (-CR[59]));
																																								Rows[6] |= LS[59];
																																								Columns[5] |= LS[59];
																																								for (CR[55] = AllN ^ (Rows[6] | Columns[1]); CR[55] != 0; CR[55] = (CR[55] & (CR[55] - 1))) {
																																									LS[55] = (CR[55] & (-CR[55]));
																																									Rows[6] |= LS[55];
																																									Columns[1] |= LS[55];
																																									for (CR[58] = AllN ^ (Rows[6] | Columns[4]); CR[58] != 0; CR[58] = (CR[58] & (CR[58] - 1))) {
																																										LS[58] = (CR[58] & (-CR[58]));
																																										Rows[6] |= LS[58];
																																										Columns[4] |= LS[58];
																																										for (CR[61] = AllN ^ (Rows[6] | Columns[7]); CR[61] != 0; CR[61] = (CR[61] & (CR[61] - 1))) {
																																											LS[61] = (CR[61] & (-CR[61]));
																																											Rows[6] |= LS[61];
																																											Columns[7] |= LS[61];
																																											for (CR[54] = AllN ^ (Rows[6] | Columns[0]); CR[54] != 0; CR[54] = (CR[54] & (CR[54] - 1))) {
																																												LS[54] = (CR[54] & (-CR[54]));
																																												Rows[6] |= LS[54];
																																												Columns[0] |= LS[54];
																																												CR[62] = AllN ^ (Rows[6] | Columns[8]);
																																												if (CR[62] != 0) {
																																													LS[62] = (CR[62] & (-CR[62]));
																																													Columns[8] |= LS[62];
																																													for (CR[66] = AllN ^ (Rows[7] | Columns[3]); CR[66] != 0; CR[66] = (CR[66] & (CR[66] - 1))) {
																																														LS[66] = (CR[66] & (-CR[66]));
																																														Rows[7] |= LS[66];
																																														Columns[3] |= LS[66];
																																														CR[39] = AllN ^ (Rows[4] | Columns[3]);
																																														if (CR[39] != 0) {
																																															LS[39] = (CR[39] & (-CR[39]));
																																															Rows[4] |= LS[39];
																																															for (CR[68] = AllN ^ (Rows[7] | Columns[5]); CR[68] != 0; CR[68] = (CR[68] & (CR[68] - 1))) {
																																																LS[68] = (CR[68] & (-CR[68]));
																																																Rows[7] |= LS[68];
																																																Columns[5] |= LS[68];
																																																CR[41] = AllN ^ (Rows[4] | Columns[5]);
																																																if (CR[41] != 0) {
																																																	LS[41] = (CR[41] & (-CR[41]));
																																																	Rows[4] |= LS[41];
																																																	for (CR[67] = AllN ^ (Rows[7] | Columns[4]); CR[67] != 0; CR[67] = (CR[67] & (CR[67] - 1))) {
																																																		LS[67] = (CR[67] & (-CR[67]));
																																																		Rows[7] |= LS[67];
																																																		Columns[4] |= LS[67];
																																																		for (CR[63] = AllN ^ (Rows[7] | Columns[0]); CR[63] != 0; CR[63] = (CR[63] & (CR[63] - 1))) {
																																																			LS[63] = (CR[63] & (-CR[63]));
																																																			Rows[7] |= LS[63];
																																																			Columns[0] |= LS[63];
																																																			for (CR[65] = AllN ^ (Rows[7] | Columns[2]); CR[65] != 0; CR[65] = (CR[65] & (CR[65] - 1))) {
																																																				LS[65] = (CR[65] & (-CR[65]));
																																																				Rows[7] |= LS[65];
																																																				Columns[2] |= LS[65];
																																																				for (CR[69] = AllN ^ (Rows[7] | Columns[6]); CR[69] != 0; CR[69] = (CR[69] & (CR[69] - 1))) {
																																																					LS[69] = (CR[69] & (-CR[69]));
																																																					Rows[7] |= LS[69];
																																																					Columns[6] |= LS[69];
																																																					CR[71] = AllN ^ (Rows[7] | Columns[8]);
																																																					if (CR[71] != 0) {
																																																						LS[71] = (CR[71] & (-CR[71]));
																																																						Columns[8] |= LS[71];
																																																						for (CR[31] = AllN ^ (Rows[3] | Columns[4]); CR[31] != 0; CR[31] = (CR[31] & (CR[31] - 1))) {
																																																							LS[31] = (CR[31] & (-CR[31]));
																																																							Rows[3] |= LS[31];
																																																							Columns[4] |= LS[31];
																																																							CR[49] = AllN ^ (Rows[5] | Columns[4]);
																																																							if (CR[49] != 0) {
																																																								LS[49] = (CR[49] & (-CR[49]));
																																																								Rows[5] |= LS[49];
																																																								for (CR[27] = AllN ^ (Rows[3] | Columns[0]); CR[27] != 0; CR[27] = (CR[27] & (CR[27] - 1))) {
																																																									LS[27] = (CR[27] & (-CR[27]));
																																																									Rows[3] |= LS[27];
																																																									Columns[0] |= LS[27];
																																																									for (CR[28] = AllN ^ (Rows[3] | Columns[1]); CR[28] != 0; CR[28] = (CR[28] & (CR[28] - 1))) {
																																																										LS[28] = (CR[28] & (-CR[28]));
																																																										Rows[3] |= LS[28];
																																																										Columns[1] |= LS[28];
																																																										for (CR[29] = AllN ^ (Rows[3] | Columns[2]); CR[29] != 0; CR[29] = (CR[29] & (CR[29] - 1))) {
																																																											LS[29] = (CR[29] & (-CR[29]));
																																																											Rows[3] |= LS[29];
																																																											Columns[2] |= LS[29];
																																																											for (CR[33] = AllN ^ (Rows[3] | Columns[6]); CR[33] != 0; CR[33] = (CR[33] & (CR[33] - 1))) {
																																																												LS[33] = (CR[33] & (-CR[33]));
																																																												Rows[3] |= LS[33];
																																																												Columns[6] |= LS[33];
																																																												for (CR[34] = AllN ^ (Rows[3] | Columns[7]); CR[34] != 0; CR[34] = (CR[34] & (CR[34] - 1))) {
																																																													LS[34] = (CR[34] & (-CR[34]));
																																																													Rows[3] |= LS[34];
																																																													Columns[7] |= LS[34];
																																																													CR[35] = AllN ^ (Rows[3] | Columns[8]);
																																																													if (CR[35] != 0) {
																																																														LS[35] = (CR[35] & (-CR[35]));
																																																														Columns[8] |= LS[35];
																																																														for (CR[36] = AllN ^ (Rows[4] | Columns[0]); CR[36] != 0; CR[36] = (CR[36] & (CR[36] - 1))) {
																																																															LS[36] = (CR[36] & (-CR[36]));
																																																															Rows[4] |= LS[36];
																																																															Columns[0] |= LS[36];
																																																															CR[45] = AllN ^ (Rows[5] | Columns[0]);
																																																															if (CR[45] != 0) {
																																																																LS[45] = (CR[45] & (-CR[45]));
																																																																Rows[5] |= LS[45];
																																																																for (CR[37] = AllN ^ (Rows[4] | Columns[1]); CR[37] != 0; CR[37] = (CR[37] & (CR[37] - 1))) {
																																																																	LS[37] = (CR[37] & (-CR[37]));
																																																																	Rows[4] |= LS[37];
																																																																	Columns[1] |= LS[37];
																																																																	CR[46] = AllN ^ (Rows[5] | Columns[1]);
																																																																	if (CR[46] != 0) {
																																																																		LS[46] = (CR[46] & (-CR[46]));
																																																																		Rows[5] |= LS[46];
																																																																		for (CR[38] = AllN ^ (Rows[4] | Columns[2]); CR[38] != 0; CR[38] = (CR[38] & (CR[38] - 1))) {
																																																																			LS[38] = (CR[38] & (-CR[38]));
																																																																			Rows[4] |= LS[38];
																																																																			Columns[2] |= LS[38];
																																																																			CR[47] = AllN ^ (Rows[5] | Columns[2]);
																																																																			if (CR[47] != 0) {
																																																																				LS[47] = (CR[47] & (-CR[47]));
																																																																				Rows[5] |= LS[47];
																																																																				for (CR[42] = AllN ^ (Rows[4] | Columns[6]); CR[42] != 0; CR[42] = (CR[42] & (CR[42] - 1))) {
																																																																					LS[42] = (CR[42] & (-CR[42]));
																																																																					Rows[4] |= LS[42];
																																																																					Columns[6] |= LS[42];
																																																																					CR[51] = AllN ^ (Rows[5] | Columns[6]);
																																																																					if (CR[51] != 0) {
																																																																						LS[51] = (CR[51] & (-CR[51]));
																																																																						Rows[5] |= LS[51];
																																																																						for (CR[43] = AllN ^ (Rows[4] | Columns[7]); CR[43] != 0; CR[43] = (CR[43] & (CR[43] - 1))) {
																																																																							LS[43] = (CR[43] & (-CR[43]));
																																																																							Rows[4] |= LS[43];
																																																																							Columns[7] |= LS[43];
																																																																							CR[44] = AllN ^ (Rows[4] | Columns[8]);
																																																																							if (CR[44] != 0) {
																																																																								LS[44] = (CR[44] & (-CR[44]));
																																																																								Columns[8] |= LS[44];
																																																																								CR[52] = AllN ^ (Rows[5] | Columns[7]);
																																																																								if (CR[52] != 0) {
																																																																									LS[52] = (CR[52] & (-CR[52]));
																																																																									Rows[5] |= LS[52];
																																																																									CR[53] = AllN ^ (Rows[5] | Columns[8]);
																																																																									if (CR[53] != 0) {
																																																																										LS[53] = (CR[53] & (-CR[53]));
																																																																										SquaresCnt++;
																																																																										if (SquaresCnt % 10000000 == 0) {
																																																																											cout << "Squares found: " << SquaresCnt << endl;

																																																																										}
																																																																									}
																																																																									Rows[5] ^= LS[52];
																																																																								}
																																																																								Columns[8] ^= LS[44];
																																																																							}
																																																																							Rows[4] ^= LS[43];
																																																																							Columns[7] ^= LS[43];
																																																																						}
																																																																						Rows[5] ^= LS[51];
																																																																					}
																																																																					Rows[4] ^= LS[42];
																																																																					Columns[6] ^= LS[42];
																																																																				}
																																																																				Rows[5] ^= LS[47];
																																																																			}
																																																																			Rows[4] ^= LS[38];
																																																																			Columns[2] ^= LS[38];
																																																																		}
																																																																		Rows[5] ^= LS[46];
																																																																	}
																																																																	Rows[4] ^= LS[37];
																																																																	Columns[1] ^= LS[37];
																																																																}
																																																																Rows[5] ^= LS[45];
																																																															}
																																																															Rows[4] ^= LS[36];
																																																															Columns[0] ^= LS[36];
																																																														}
																																																														Columns[8] ^= LS[35];
																																																													}
																																																													Rows[3] ^= LS[34];
																																																													Columns[7] ^= LS[34];
																																																												}
																																																												Rows[3] ^= LS[33];
																																																												Columns[6] ^= LS[33];
																																																											}
																																																											Rows[3] ^= LS[29];
																																																											Columns[2] ^= LS[29];
																																																										}
																																																										Rows[3] ^= LS[28];
																																																										Columns[1] ^= LS[28];
																																																									}
																																																									Rows[3] ^= LS[27];
																																																									Columns[0] ^= LS[27];
																																																								}
																																																								Rows[5] ^= LS[49];
																																																							}
																																																							Rows[3] ^= LS[31];
																																																							Columns[4] ^= LS[31];
																																																						}
																																																						Columns[8] ^= LS[71];
																																																					}
																																																					Rows[7] ^= LS[69];
																																																					Columns[6] ^= LS[69];
																																																				}
																																																				Rows[7] ^= LS[65];
																																																				Columns[2] ^= LS[65];
																																																			}
																																																			Rows[7] ^= LS[63];
																																																			Columns[0] ^= LS[63];
																																																		}
																																																		Rows[7] ^= LS[67];
																																																		Columns[4] ^= LS[67];
																																																	}
																																																	Rows[4] ^= LS[41];
																																																}
																																																Rows[7] ^= LS[68];
																																																Columns[5] ^= LS[68];
																																															}
																																															Rows[4] ^= LS[39];
																																														}
																																														Rows[7] ^= LS[66];
																																														Columns[3] ^= LS[66];
																																													}
																																													Columns[8] ^= LS[62];
																																												}
																																												Rows[6] ^= LS[54];
																																												Columns[0] ^= LS[54];
																																											}
																																											Rows[6] ^= LS[61];
																																											Columns[7] ^= LS[61];
																																										}
																																										Rows[6] ^= LS[58];
																																										Columns[4] ^= LS[58];
																																									}
																																									Rows[6] ^= LS[55];
																																									Columns[1] ^= LS[55];
																																								}
																																								Rows[6] ^= LS[59];
																																								Columns[5] ^= LS[59];
																																							}
																																							Rows[6] ^= LS[57];
																																							Columns[3] ^= LS[57];
																																						}
																																						Columns[8] ^= LS[26];
																																					}
																																					Rows[2] ^= LS[18];
																																					Columns[0] ^= LS[18];
																																				}
																																				Rows[2] ^= LS[25];
																																				Columns[7] ^= LS[25];
																																			}
																																			Rows[2] ^= LS[22];
																																			Columns[4] ^= LS[22];
																																		}
																																		Rows[2] ^= LS[19];
																																		Columns[1] ^= LS[19];
																																	}
																																	Rows[2] ^= LS[23];
																																	Columns[5] ^= LS[23];
																																}
																																Rows[2] ^= LS[21];
																																Columns[3] ^= LS[21];
																															}
																															Columns[8] ^= LS[17];
																														}
																														Rows[1] ^= LS[9];
																														Columns[0] ^= LS[9];
																													}
																													Rows[1] ^= LS[13];
																													Columns[4] ^= LS[13];
																												}
																												Rows[1] ^= LS[15];
																												Columns[6] ^= LS[15];
																											}
																											Rows[1] ^= LS[14];
																											Columns[5] ^= LS[14];
																										}
																										Rows[1] ^= LS[12];
																										Columns[3] ^= LS[12];
																									}
																									Rows[1] ^= LS[11];
																									Columns[2] ^= LS[11];
																								}
																								Columns[7] ^= LS[79];
																							}

																						}
																						Rows[8] ^= LS[78];
																						Columns[6] ^= LS[78];
																					}
																					Rows[8] ^= LS[77];
																					Columns[5] ^= LS[77];
																				}
																				Rows[8] ^= LS[76];
																				Columns[4] ^= LS[76];
																			}
																			Rows[8] ^= LS[75];
																			Columns[3] ^= LS[75];
																		}
																		Rows[8] ^= LS[74];
																		Columns[2] ^= LS[74];
																	}
																	Rows[8] ^= LS[73];
																	Columns[1] ^= LS[73];
																}
																Rows[8] ^= LS[80];
																Columns[8] ^= LS[80];

																//}
															}
															Rows[5] ^= LS[50];
															Columns[5] ^= LS[50];
															MD ^= LS[50];
														}
														Rows[8] ^= LS[72];
														Columns[0] ^= LS[72];
													}
													Rows[5] ^= LS[48];
													Columns[3] ^= LS[48];
													AD ^= LS[48];
												}
												Rows[3] ^= LS[32];
												Columns[5] ^= LS[32];
												AD ^= LS[32];
											}
											Rows[3] ^= LS[30];
											Columns[3] ^= LS[30];
											MD ^= LS[30];
										}
										Rows[6] ^= LS[60];
										Columns[6] ^= LS[60];
										MD ^= LS[60];
									}
									Rows[6] ^= LS[56];
									Columns[2] ^= LS[56];
									AD ^= LS[56];
								}
								Rows[2] ^= LS[24];
								Columns[6] ^= LS[24];
								AD ^= LS[24];
							}
							Rows[2] ^= LS[20];
							Columns[2] ^= LS[20];
							MD ^= LS[20];
						}
						Rows[7] ^= LS[70];
						Columns[7] ^= LS[70];
						MD ^= LS[70];
					}
					Rows[7] ^= LS[64];
					Columns[1] ^= LS[64];
					AD ^= LS[64];
				}
				Rows[1] ^= LS[16];
				Columns[7] ^= LS[16];
				AD ^= LS[16];
			}
			Rows[1] ^= LS[10];
			Columns[1] ^= LS[10];
			MD ^= LS[10];
		}
		Rows[4] ^= LS[40];
		Columns[4] ^= LS[40];
		MD ^= LS[40];
		AD ^= LS[40];
	}
	TotalCnt += SquaresCnt*multiple;

	
	return TotalCnt;
	//	return SquaresCnt;
}



