using namespace std;
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

void gen_inner_cycle_Eduard(int i, vector<int> &order, ofstream &out) {
	int N = 9;
	int t = order[i];
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << s << "for (LS[" << t << "] = 0; LS[" << t << "]< N; LS[" << t << "]++) {\n";
	out << s << "\t if (!Strs[" << t / N << "][LS[" << t << "]] || !Rows[" << t % N << "][LS[" << t << "]]";
	if (t % N == t / N) out << "|| !d1[LS[" << t << "]]";
	if (t % N == (N - (t / N) - 1)) out << "|| !d2[LS[" << t << "]]";
	out << ")\n";
	out << s << "\t\tcontinue;\n\n";

	out << s << "Strs[" << t / N << "][LS[" << t << "]] = 0;\n";
	out << s << "Rows[" << t % N << "][LS[" << t << "]] = 0;\n";
	if (t % N == t / N) out << "d1[LS[" << t << "]] = 0;\n";
	if (t % N == (N - (t / N) - 1)) out << "d2[LS[" << t << "]] = 0;\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_Eduard(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}
	out << s << "Strs[" << t / N << "][LS[" << t << "]] = 1;\n";
	out << s << "Rows[" << t % N << "][LS[" << t << "]] = 1;\n";
	if (t % N == t / N) out << "d1[LS[" << t << "]] = 1;\n";
	if (t % N == (N - (t / N) - 1)) out << "d2[LS[" << t << "]] = 1;\n";
	out << "}\n";
}


void gen_code_Eduard_rc0() {
	ofstream out;

	out.open("D:\\LStests\\Eduard.cpp");

	
	out << " #include <iostream>\n";

	out << " #include <chrono>\n";
	out << "using namespace std;\n";

	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";



	out << "const int N=9;\n";

	out << "int LS[N*N];\n";

	out << "int SquaresCnt = 0;\n";

	out << "int Rows[N][N], Strs[N][N], d1[N], d2[N];\n";

	out << "__int64 ProcFreq = 3.4e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=-1;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=i;\n";

	out << "for (int i = 0; i < N; i++)\n\tfor (int j = 0; j < N; j++)\n\t\t Strs[i][j]=1;\n";

	out << "for (int i = 0; i < N; i++){\n\tfor (int j = 0; j < N; j++)\n\t\t Rows[i][j]=1;\n\n \t Rows[i][i]=0;\n}\n";



	out << "d1[0] = 0;\n for (int i = 1; i <N;i++)\n\td1[i]=1;\n";

	out << "d2[N-1] = 0;\n for (int i = 0; i <N-1;i++)\n\td2[i]=1;\n";

	

	out << "}\n\n";

	out << "void Generate();\n\n";



	out << "void main()\n\{\n";

	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";
	vector<int> order{
		40, 10, 16, 64, 70, 20, 24, 56, 60,
		30, 32, 48, 72, 50, 80, 11, 12, 14,
		15, 9, 13, 17, 21, 23, 18, 19, 22,
		25, 26, 57, 59, 54, 55, 58, 61, 62,
		66, 68, 63, 67, 71, 65, 69, 75, 39,
		77, 41, 76, 73, 74, 78, 79, 31, 49,
		27, 28, 29, 33, 34, 35, 36, 45, 37,
		46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80};
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	gen_inner_cycle_Eduard(0, order, out);
	out << "}\n";

	out.close();

}


void gen_inner_cycle_Eduard_BT(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << "CR[" << t << "] = Rows[" << t / 10 << "] | Columns[" << t % 10 << "]";
	if (t % 10 == t / 10) out << "| MD ";
	if (t % 10 == (10 - (t / 10) - 1)) out << "| AD";
	out << ";\n";

	out << s << "for (LS[" << t << "] = 1; LS[" << t << "]< maxN; LS[" << t << "]= LS[" << t << "] << 1) {\n";
	out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
	out << s << "\t\tcontinue;\n\n";

	out << s << "Rows[" << t / 10 << "] |= LS[" << t << "];\n";
	out << s << "Columns[" << t % 10 << "] |= LS[" << t << "];\n";
	if (t % 10 == t / 10) out << "MD |= LS[" << t << "];\n";
	if (t % 10 == (10 - (t / 10) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_Eduard_BT(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	out << s << "Rows[" << t / 10 << "] ^= LS[" << t << "];\n";
	out << s << "Columns[" << t % 10 << "] ^= LS[" << t << "];\n";
	if (t % 10 == t / 10) out << "MD ^= LS[" << t << "];\n";
	if (t % 10 == (10 - (t / 10) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}


void gen_inner_cycle_Eduard_BT_rc1(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << "CR[" << t << "] = Rows[" << t / 10 << "] | Columns[" << t % 10 << "]";
	if (t % 10 == t / 10) out << "| MD ";
	if (t % 10 == (10 - (t / 10) - 1)) out << "| AD";
	out << ";\n";

	out << "L[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));\n";
	out << s << "for (LS[" << t << "] = L[" << t << "]; ((L[" << t << "]>0)&&(LS[" << t << "]< maxN)); LS[" << t << "]= LS[" << t << "] << 1) {\n";
	out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
	out << s << "\t\tcontinue;\n\n";

	out << s << "Rows[" << t / 10 << "] |= LS[" << t << "];\n";
	out << s << "Columns[" << t % 10 << "] |= LS[" << t << "];\n";
	if (t % 10 == t / 10) out << "MD |= LS[" << t << "];\n";
	if (t % 10 == (10 - (t / 10) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_Eduard_BT_rc1(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	out << s << "Rows[" << t / 10 << "] ^= LS[" << t << "];\n";
	out << s << "Columns[" << t % 10 << "] ^= LS[" << t << "];\n";
	if (t % 10 == t / 10) out << "MD ^= LS[" << t << "];\n";
	if (t % 10 == (10 - (t / 10) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}




void gen_code_Eduard_BT_rc0() {
	ofstream out;

	out.open("D:\\LStests\\Eduard_BT.cpp");
	out << " #include <iostream>\n";

	out << " #include <chrono>\n";

	out << "using namespace std;\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";

	out << "const int N=10;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 1023;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 2.16e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "\n int known_rows[4][10]{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 2, 3, 4, 9, 8, 1, 0, 5, 6, 7 },{ 3, 4, 9, 8, 2, 7, 1, 0, 5, 6 },	{ 8, 7, 6, 5, 0, 9, 4, 3, 2, 1 }};\n";



	out << "for (int i=0;i<4;i++)\n\tfor (int j=0;j<N;j++){\n";

	out << "\t\tLS[i*N+j] = 1<<known_rows[i][j];\n\t\t	Rows[i]|=LS[i*N+j];\n\t\tColumns[j]|= LS[i*N+j];\n";

	out << "\t\tif (i==j){\n\t\t\MD|=LS[i*N+j];\n\t}\n";

	out << "\t\tif (i == N - j - 1) {\n\t\t\AD|=LS[i*N+j];\n\t}\n";

	out << "\t}\n";

	out << "}\n\n";

	out << "void Generate();\n\n";



	out << "void main()\n\{\n";

	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	gen_inner_cycle_Eduard_BT(0, order, out);
	out << "}\n";

	out.close();

}


void gen_code_Eduard_BT_rc1() {
	ofstream out;

	out.open("C:\\Work\\tests\\Eduard_BT_rc1.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=10;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 1023;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 2.16e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "\n int known_rows[4][10]{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 2, 3, 4, 9, 8, 1, 0, 5, 6, 7 },{ 3, 4, 9, 8, 2, 7, 1, 0, 5, 6 },	{ 8, 7, 6, 5, 0, 9, 4, 3, 2, 1 }};\n";



	out << "for (int i=0;i<4;i++)\n\tfor (int j=0;j<N;j++){\n";

	out << "\t\tLS[i*N+j] = 1<<known_rows[i][j];\n\t\t	Rows[i]|=LS[i*N+j];\n\t\tColumns[j]|= LS[i*N+j];\n";

	out << "\t\tif (i==j){\n\t\t\MD|=LS[i*N+j];\n\t}\n";

	out << "\t\tif (i == N - j - 1) {\n\t\t\AD|=LS[i*N+j];\n\t}\n";

	out << "\t}\n";

	out << "}\n\n";

	out << "void Generate();\n\n";



	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_Eduard_BT_rc1(0, order, out);
	out << "}\n";

	out.close();

}



void gen_inner_cycle_Eduard_BT_rc2(int i, vector<int> &order, vector<int> &status, ofstream &out) {
	if (status[order[i]] == 0) {
		int t = order[i];

		int t_i = t / 10;
		int t_j = t % 10;

		int cnt_r = 0;
		int cnt_c = 4;

		vector<int> EXP;

		vector<int> KN_LS(100);
		vector<int> KN_R_CUR(100);
		vector<int> KN_C_CUR(100);
		status[order[i]] = 1;

		for (auto j = 0; j < 40; j++) {
			KN_LS[j] = 1;
		}
		for (auto j = 0; j < i; j++) {
			KN_LS[order[j]] = 1;
		}

		for (auto u = 0; u < 10; u++) {
			for (auto v = 0; v < 10; v++) {
				for (auto j = 0; j < 10; j++) {

					//if (v != j)
					KN_R_CUR[u * 10 + v] += KN_LS[u * 10 + j];
					//if (u != j) 
					KN_C_CUR[u * 10 + v] += KN_LS[j * 10 + v];
				}
			}
		}
		if (KN_R_CUR[t] == 9)
		{
			if (status[order[i]] == 0) {
				cout << "ROW___" << order[i] << "@" << i << "\n";
			}
		}

		if (KN_C_CUR[t] == 9)
		{
			if (status[order[i]] == 0) {
				cout << "COL___" << order[i] << "@" << i << "\n";
			}
		}

		vector<int> KN_R_NX(KN_R_CUR);
		vector<int> KN_C_NX(KN_C_CUR);



		for (auto j = 0; j < 10; j++) {
			KN_R_NX[t_i * 10 + j]++;
			KN_C_NX[10 * j + t_j]++;
		}
		KN_LS[order[i]] = 1;

		for (auto j = 40; j < 100; j++) {
			//if ((KN_R_NX[j] <10) && (KN_C_NX[j] < 10)&& (KN_R_NX[j] >= 8) && (KN_C_NX[j] >=8)) {
			if (((KN_R_NX[j] == 9) || (KN_C_NX[j] == 9)) && (KN_LS[j] == 0) && (status[j] == 0)) {
				cout << "R|C" << j << "@" << i << "\n";
				status[j] = 1;
				EXP.push_back(j);
			}
			/*if (KN_R_NX[j] == 9)
			{
			cout << "THIS@"<<i<<"\n";
			}
			if (KN_C_NX[j] == 9)
			{
			cout << "THIS@" << i << "\n";
			}*/
		}
		int md_sum = 0;
		int ad_sum = 0;
		for (auto j = 0; j < 10; j++) {
			md_sum += KN_LS[j * 10 + j];
			ad_sum += KN_LS[j * 10 + (10 - j - 1)];
		}
		if (md_sum == 9) {
			for (auto j = 0; j < 10; j++) {
				if (KN_LS[j * 10 + j] == 0) {
					if (status[j * 10 + j] == 0) {
						status[j * 10 + j] = 1;
						EXP.push_back(j * 10 + j);
						cout << "MD" << j * 10 + j << "@" << i << "\n";
					}
				}
			}
		}

		if (ad_sum == 9) {
			for (auto j = 0; j < 10; j++) {
				if (KN_LS[j * 10 + (10 - j - 1)] == 0) {
					if (status[j * 10 + (10 - j - 1)] == 0) {
						status[j * 10 + (10 - j - 1)] = 1;
						EXP.push_back(j * 10 + (10 - j - 1));
						cout << "AD" << j * 10 + (10 - j - 1) << "@" << i << "\n";
					}
				}
			}
		}




		out << "CR[" << t << "] = Rows[" << t / 10 << "] | Columns[" << t % 10 << "]";
		if (t % 10 == t / 10) out << "| MD ";
		if (t % 10 == (10 - (t / 10) - 1)) out << "| AD";
		out << ";\n";

		out << "L[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));\n";
		out << "for (LS[" << t << "] = L[" << t << "]; ((L[" << t << "]>0)&&(LS[" << t << "]< maxN)); LS[" << t << "]= LS[" << t << "] << 1) {\n";
		out << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
		out << "\t\tcontinue;\n\n";

		out << "Rows[" << t / 10 << "] |= LS[" << t << "];\n";
		out << "Columns[" << t % 10 << "] |= LS[" << t << "];\n";
		if (t % 10 == t / 10) out << "MD |= LS[" << t << "];\n";
		if (t % 10 == (10 - (t / 10) - 1)) out << "AD |=LS[" << t << "];\n";

		if (EXP.size() != 0) {
			for (auto j = 0; j < EXP.size(); j++) {
				out << "CR[" << EXP[j] << "] = Rows[" << EXP[j] / 10 << "] | Columns[" << EXP[j] % 10 << "]";
				if (EXP[j] % 10 == EXP[j] / 10) out << "| MD ";
				if (EXP[j] % 10 == (10 - (EXP[j] / 10) - 1)) out << "| AD";
				out << ";\n";
				out << "L[" << EXP[j] << "] = CR[" << EXP[j] << "] ^ AllN;\n";
			}
			out << "if (";
			for (auto j = 0; j < EXP.size(); j++) {
				out << "(L[" << EXP[j] << "] != 0) ";
				if (EXP.size() - j>1) out << " && ";
			}
			out << " ) {\n";
			for (auto j = 0; j < EXP.size(); j++) {
				out << "LS[" << EXP[j] << "] = L[" << EXP[j] << "];\n";
				out << "Rows[" << EXP[j] / 10 << "] |= LS[" << EXP[j] << "];\n";
				out << "Columns[" << EXP[j] % 10 << "] |= LS[" << EXP[j] << "];\n";
				if (EXP[j] % 10 == EXP[j] / 10) out << "MD |= LS[" << EXP[j] << "];\n";
				if (EXP[j] % 10 == (10 - (EXP[j] / 10) - 1)) out << "AD |=LS[" << EXP[j] << "];\n";
			}
			gen_inner_cycle_Eduard_BT_rc2(i + 1, order, status, out);

			for (auto j = 0; j < EXP.size(); j++) {
				out << "Rows[" << EXP[j] / 10 << "] ^= LS[" << EXP[j] << "];\n";
				out << "Columns[" << EXP[j] % 10 << "] ^= LS[" << EXP[j] << "];\n";
				if (EXP[j] % 10 == EXP[j] / 10) out << "MD ^= LS[" << EXP[j] << "];\n";
				if (EXP[j] % 10 == (10 - (EXP[j] / 10) - 1)) out << "AD ^=LS[" << EXP[j] << "];\n";
			}

			out << "}\n";
			out << "Rows[" << t / 10 << "] ^= LS[" << t << "];\n";
			out << "Columns[" << t % 10 << "] ^= LS[" << t << "];\n";
			if (t % 10 == t / 10) out << "MD ^= LS[" << t << "];\n";
			if (t % 10 == (10 - (t / 10) - 1)) out << "AD ^=LS[" << t << "];\n";
			out << "}\n";
		}
		else {

			if (i < order.size() - 1) {
				gen_inner_cycle_Eduard_BT_rc2(i + 1, order, status, out);
			}
			else {
				out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
				out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
				out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
				out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
				out << "}\n";
			}
			//out << "loop" << t << "end:\n";
			out << "Rows[" << t / 10 << "] ^= LS[" << t << "];\n";
			out << "Columns[" << t % 10 << "] ^= LS[" << t << "];\n";
			if (t % 10 == t / 10) out << "MD ^= LS[" << t << "];\n";
			if (t % 10 == (10 - (t / 10) - 1)) out << "AD ^=LS[" << t << "];\n";
			out << "}\n";
		}
	}
	else {
		gen_inner_cycle_Eduard_BT_rc2(i + 1, order, status, out);
	}
}




void gen_code_Eduard_BT_rc2() {
	ofstream out;

	out.open("C:\\Work\\tests\\Eduard_BT_rc2.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=10;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 1023;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "\n int known_rows[4][10]{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 2, 3, 4, 9, 8, 1, 0, 5, 6, 7 },{ 3, 4, 9, 8, 2, 7, 1, 0, 5, 6 },	{ 8, 7, 6, 5, 0, 9, 4, 3, 2, 1 }};\n";



	out << "for (int i=0;i<4;i++)\n\tfor (int j=0;j<N;j++){\n";

	out << "\t\tLS[i*N+j] = 1<<known_rows[i][j];\n\t\t	Rows[i]|=LS[i*N+j];\n\t\tColumns[j]|= LS[i*N+j];\n";

	out << "\t\tif (i==j){\n\t\t\MD|=LS[i*N+j];\n\t}\n";

	out << "\t\tif (i == N - j - 1) {\n\t\t\AD|=LS[i*N+j];\n\t}\n";

	out << "\t}\n";

	out << "}\n\n";

	out << "void Generate();\n\n";



	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	vector<int> status(100);
	for (int i = 0; i < 40; i++) {
		status[i] = 1;
	}
	gen_inner_cycle_Eduard_BT_rc2(0, order, status, out);
	out << "}\n";

	out.close();

}



void gen_inner_cycle_Eduard_BT9_rc0(int i, vector<int> &order, vector<int> &status, ofstream &out) {
	if (status[order[i]] == 0) {
		int t = order[i];
		int N = 9;
		int t_i = t / N;
		int t_j = t % N;


		vector<int> EXP;

		vector<int> KN_LS(N*N);
		vector<int> KN_R_CUR(N*N);
		vector<int> KN_C_CUR(N*N);
		status[order[i]] = 1;

		for (auto j = 0; j < N; j++) {
			KN_LS[j] = 1;
		}
		for (auto j = 0; j < i; j++) {
			KN_LS[order[j]] = 1;
		}

		for (auto u = 0; u < N; u++) {
			for (auto v = 0; v < N; v++) {
				for (auto j = 0; j < N; j++) {

					//if (v != j)
					KN_R_CUR[u * N + v] += KN_LS[u * N + j];
					//if (u != j) 
					KN_C_CUR[u * N + v] += KN_LS[j * N + v];
				}
			}
		}
		if (KN_R_CUR[t] == N - 1)
		{
			if (status[order[i]] == 0) {
				cout << "ROW___" << order[i] << "@" << i << "\n";
			}
		}

		if (KN_C_CUR[t] == N - 1)
		{
			if (status[order[i]] == 0) {
				cout << "COL___" << order[i] << "@" << i << "\n";
			}
		}

		vector<int> KN_R_NX(KN_R_CUR);
		vector<int> KN_C_NX(KN_C_CUR);



		for (auto j = 0; j < N; j++) {
			KN_R_NX[t_i * N + j]++;
			KN_C_NX[N * j + t_j]++;
		}
		KN_LS[order[i]] = 1;

		for (auto j = N; j < N*N; j++) {
			//if ((KN_R_NX[j] <10) && (KN_C_NX[j] < 10)&& (KN_R_NX[j] >= 8) && (KN_C_NX[j] >=8)) {
			if (((KN_R_NX[j] == N - 1) || (KN_C_NX[j] == N - 1)) && (KN_LS[j] == 0) && (status[j] == 0)) {
				cout << "R|C" << j << "@" << i << "\n";
				status[j] = 1;
				EXP.push_back(j);
			}
			/*if (KN_R_NX[j] == 9)
			{
			cout << "THIS@"<<i<<"\n";
			}
			if (KN_C_NX[j] == 9)
			{
			cout << "THIS@" << i << "\n";
			}*/
		}
		int md_sum = 0;
		int ad_sum = 0;
		for (auto j = 0; j < N; j++) {
			md_sum += KN_LS[j * N + j];
			ad_sum += KN_LS[j * N + (N - j - 1)];
		}
		if (md_sum == 8) {
			for (auto j = 0; j < N; j++) {
				if (KN_LS[j * N + j] == 0) {
					if (status[j * N + j] == 0) {
						status[j * N + j] = 1;
						EXP.push_back(j * N + j);
						cout << "MD" << j * N + j << "@" << i << "\n";
					}
				}
			}
		}

		if (ad_sum == 8) {
			for (auto j = 0; j < N; j++) {
				if (KN_LS[j * N + (N - j - 1)] == 0) {
					if (status[j * N + (N - j - 1)] == 0) {
						status[j * N + (N - j - 1)] = 1;
						EXP.push_back(j * N + (N - j - 1));
						cout << "AD" << j * N + (N - j - 1) << "@" << i << "\n";
					}
				}
			}
		}




		out << "CR[" << t << "] = Rows[" << t / N << "] | Columns[" << t % N << "]";
		if (t % N == t / N) out << "| MD ";
		if (t % N == (N - (t / N) - 1)) out << "| AD";
		out << ";\n";

		out << "L[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));\n";
		out << "for (LS[" << t << "] = L[" << t << "]; ((L[" << t << "]>0)&&(LS[" << t << "]< maxN)); LS[" << t << "]= LS[" << t << "] << 1) {\n";
		out << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
		out << "\t\tcontinue;\n\n";

		out << "Rows[" << t / N << "] |= LS[" << t << "];\n";
		out << "Columns[" << t % N << "] |= LS[" << t << "];\n";
		if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
		if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

		if (EXP.size() != 0) {
			for (auto j = 0; j < EXP.size(); j++) {
				out << "CR[" << EXP[j] << "] = Rows[" << EXP[j] / N << "] | Columns[" << EXP[j] % N << "]";
				if (EXP[j] % N == EXP[j] / N) out << "| MD ";
				if (EXP[j] % N == (N - (EXP[j] / N) - 1)) out << "| AD";
				out << ";\n";
				out << "L[" << EXP[j] << "] = CR[" << EXP[j] << "] ^ AllN;\n";
			}
			out << "if (";
			for (auto j = 0; j < EXP.size(); j++) {
				out << "(L[" << EXP[j] << "] != 0) ";
				if (EXP.size() - j>1) out << " && ";
			}
			out << " ) {\n";
			for (auto j = 0; j < EXP.size(); j++) {
				out << "LS[" << EXP[j] << "] = L[" << EXP[j] << "];\n";
				out << "Rows[" << EXP[j] / N << "] |= LS[" << EXP[j] << "];\n";
				out << "Columns[" << EXP[j] % N << "] |= LS[" << EXP[j] << "];\n";
				if (EXP[j] % N == EXP[j] / N) out << "MD |= LS[" << EXP[j] << "];\n";
				if (EXP[j] % N == (N - (EXP[j] / N) - 1)) out << "AD |=LS[" << EXP[j] << "];\n";
			}
			gen_inner_cycle_Eduard_BT9_rc0(i + 1, order, status, out);

			for (auto j = 0; j < EXP.size(); j++) {
				out << "Rows[" << EXP[j] / N << "] ^= LS[" << EXP[j] << "];\n";
				out << "Columns[" << EXP[j] % N << "] ^= LS[" << EXP[j] << "];\n";
				if (EXP[j] % N == EXP[j] / N) out << "MD ^= LS[" << EXP[j] << "];\n";
				if (EXP[j] % N == (N - (EXP[j] / N) - 1)) out << "AD ^=LS[" << EXP[j] << "];\n";
			}

			out << "}\n";
			out << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
			out << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
			if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
			if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
			out << "}\n";
		}
		else {

			if (i < order.size() - 1) {
				gen_inner_cycle_Eduard_BT9_rc0(i + 1, order, status, out);
			}
			else {
				out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
				out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
				out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
				out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
				out << "}\n";
			}
			//out << "loop" << t << "end:\n";
			out << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
			out << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
			if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
			if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
			out << "}\n";
		}
	}
	else {
		gen_inner_cycle_Eduard_BT9_rc0(i + 1, order, status, out);
	}
}




void gen_code_Eduard_BT9_rc0() {
	ofstream out;

	out.open("D:\\LStests\\Eduard_BT9_rc0.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=9;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 511;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "\tLS[j] = 1<<j;\n\t\t	Rows[0]|=LS[j];\n\tColumns[j]|= LS[j];\n}\n";

	out << "MD|=LS[0];\n";

	out << "AD|=LS[N-1];\n";

	out << "}\n";


	out << "void Generate();\n\n";



	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	vector<int> status(81);
	for (int i = 0; i < 9; i++) {
		status[i] = 1;
	}
	gen_inner_cycle_Eduard_BT9_rc0(0, order, status, out);
	out << "}\n";

	out.close();

}


void gen_inner_cycle_9BT_rc2(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	int N = 9;
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << "CR[" << t << "] = Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (t % N == t / N) out << "| MD ";
	if (t % N == (N - (t / N) - 1)) out << "| AD";
	out << ";\n";
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	//out << "L[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));\n";
	out << s << "for (LS[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));";
	out << "LS[" << t << "] > 0; CR[" << t << "] ^= LS[" << t << "], LS[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN))) {\n";
	//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
	//out << s << "\t\tcontinue;\n\n";

	out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_9BT_rc2(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
	if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}





void gen_code9BT_rc2() {
	ofstream out;

	out.open("C:\\Work\\tests\\LS_GEN9BT_rc2.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=9;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 511;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	out << "MD|=LS[0];\n";
	out << "AD|=LS[N-1];\n";

	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_9BT_rc2(0, order9, out);
	out << "}\n";

	out.close();

}


void gen_inner_cycle_10BT_rc3(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	int N = 10;
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << "CR[" << t << "] = Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (t % N == t / N) out << "| MD ";
	if (t % N == (N - (t / N) - 1)) out << "| AD";
	out << ";\n";
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	//out << "L[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));\n";
	out << s << "for (LS[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN));";
	out << "LS[" << t << "] > 0; CR[" << t << "] ^= LS[" << t << "], LS[" << t << "] = (CR[" << t << "]^AllN)&(-(CR[" << t << "]^AllN))) {\n";
	//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
	//out << s << "\t\tcontinue;\n\n";

	out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_10BT_rc3(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
	if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}





void gen_code10BT_rc3() {
	ofstream out;

	out.open("D:\\LStests\\LSGEN10_BTrc3.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=10;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 1023;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "\n int known_rows[4][10]{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 2, 3, 4, 9, 8, 1, 0, 5, 6, 7 },{ 3, 4, 9, 8, 2, 7, 1, 0, 5, 6 },	{ 8, 7, 6, 5, 0, 9, 4, 3, 2, 1 }};\n";



	out << "for (int i=0;i<4;i++)\n\tfor (int j=0;j<N;j++){\n";

	out << "\t\tLS[i*N+j] = 1<<known_rows[i][j];\n\t\t	Rows[i]|=LS[i*N+j];\n\t\tColumns[j]|= LS[i*N+j];\n";

	out << "\t\tif (i==j){\n\t\t\MD|=LS[i*N+j];\n\t}\n";

	out << "\t\tif (i == N - j - 1) {\n\t\t\AD|=LS[i*N+j];\n\t}\n";

	out << "\t}\n";

	out << "}\n\n";

	out << "void Generate();\n\n";



	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	vector<int> status(100);
	for (int i = 0; i < 40; i++) {
		status[i] = 1;
	}
	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_10BT_rc3(0, order, out);
	out << "}\n";

	out.close();

}

void gen_inner_cycle_9BT_rc3(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	int N = 9;
	vector<int> KN_LS(N*N);
	for (auto j = 0; j < N; j++) {
		KN_LS[j] = 1;
	}
	for (auto j = 0; j <= i; j++) {
		KN_LS[order[j]] = 1;
	}
	bool fullR = false;
	bool fullC = false;
	bool fullMD = false;
	bool fullAD = false;

	int nR = 0;
	int nC = 0;
	int nMD = 0;
	int nAD = 0;
	bool has9 = false;
	for (auto j = 0; j < N; j++) {
		nR += KN_LS[(t / N)*N + j];
		nC += KN_LS[j*N + t % N];
		if (t%N == t / N) {
			nMD += KN_LS[j*N + j];
		}
		if ((t%N) + (t / N) == N - 1) {
			nAD += KN_LS[j*N + (N - j - 1)];
		}
	}

	if ((nR == N) || (nC == N) || (nMD == N) || (nAD == N)) {
		for (auto u = 0; u < N*N; u++) {
			cout << KN_LS[u] << " ";
			if ((u + 1) % N == 0) cout << endl;
		}
		has9 = true;
		if (nR == N) {
			fullR = true;
		}
		if (nC == N) {
			fullC = true;
		}
		if (nMD == N) {
			fullMD = true;
		}
		if (nAD == N) {
			fullAD = true;
		}
		cout << "Has 9 @" << i << " (" << order[i] << ")" << endl;
	}

	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}

	if (has9 == false) {
		out << "for (";
	}


	out << "CR[" << t << "] =  Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (t % N == t / N) out << "| MD ";
	if (t % N == (N - (t / N) - 1)) out << "| AD";
	out << ";";
	if (has9) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=AllN){\n";

		out << "LS[" << t << "] = (~CR[" << t << "] & (CR[" << t << "] + 1));\n";
		//		out << s << "if (LS[" << t << "] <MaxN){\n";

	}
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	//	out << "LS[" << t << "] = (!CR[" << t << "] & (CR[" << t << "] + 1));\n";
	// for (CR[t] = Rows[t]|Columns[t]; CR[t]!=AllN; CR[t]=CR[t]|(CR[t]+1){
	//LS[t]=(~CR[t] &(CR[t]+1);

	//L[44] = CR[44] ^ AllN; 
	//for (LS[44] = (L[44])&(-L[44]); LS[44] > 0; L[44] ^= LS[44], LS[44] = (L[44])&(-L[44])) {
	else {
		out << " CR[" << t << "] != AllN; CR[" << t << "] = (CR[" << t << "]|(CR[" << t << "] +1))){\n";
		out << "LS[" << t << "] = ~CR[" << t << "] & (CR[" << t << "] + 1);\n";
		//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
		//out << s << "\t\tcontinue;\n\n";
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_9BT_rc3(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	if (fullR == false) out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}





void gen_code9BT_rc3(vector<int> order9, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=9;\n";

	out << "const uint32_t MaxN=1<<(N+1);\n";

	out << "const uint32_t AllN= 511;\n";

	out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	//out << "int32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	out << "MD|=LS[0];\n";
	out << "AD|=LS[N-1];\n";

	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_9BT_rc3(0, order9, out);
	out << "}\n";

	out.close();

}


void gen_inner_cycle_9BT_rc4(int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	int N = 9;
	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}
	out << "CR[order9[" << i << "]] = Rows[order9[" << i << "] / N]| Columns[order9[" << i << "] % N];\n";
	out << " if (order9[" << i << "]%N == order9[" << i << "]/N) CR[order9[" << i << "]]|=MD;\n";
	out << " if (order9[" << i << "]%N == (N-order9[" << i << "]/N)-1) CR[order9[" << i << "]]|=AD;\n";
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	out << "L[order9[" << i << "]] = CR[order9[" << i << "]]^AllN;\n";

	//L[44] = CR[44] ^ AllN; 
	//for (LS[44] = (L[44])&(-L[44]); LS[44] > 0; L[44] ^= LS[44], LS[44] = (L[44])&(-L[44])) {

	out << s << "for (LS[order9[" << i << "]] = L[order9[" << i << "]]&(-L[order9[" << i << "]]);";
	out << "LS[order9[" << i << "]] > 0; L[order9[" << i << "]] ^= LS[order9[" << i << "]], LS[order9[" << i << "]] = L[order9[" << i << "]]&(-L[order9[" << i << "]])) {\n";
	//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
	//out << s << "\t\tcontinue;\n\n";

	out << s << "Rows[order9[" << i << "] /N] |= LS[order9[" << i << "\]];\n";
	out << s << "Columns[order9[" << i << "] % N] |= LS[order9[" << i << "]];\n";
	out << " if (order9[" << i << "]%N == order9[" << i << "]/N) MD |= LS[order9[" << i << "]]; \n";
	out << " if (order9[" << i << "]%N == (N-order9[" << i << "]/N)-1) AD |= LS[order9[" << i << "]]; \n";

	if (i < order.size() - 1) {
		gen_inner_cycle_9BT_rc4(i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 1000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	out << s << "Rows[order9[" << i << "] /N] ^= LS[order9[" << i << "]];\n";
	out << s << "Columns[order9[" << i << "] % N] ^= LS[order9[" << i << "]];\n";
	out << " if (order9[" << i << "]%N == order9[" << i << "]/N) MD ^= LS[order9[" << i << "]]; \n";
	out << " if (order9[" << i << "]%N == (N-order9[" << i << "]/N)-1) AD ^= LS[order9[" << i << "]]; \n";
	out << "}\n";
}





void gen_code9BT_rc4() {
	ofstream out;

	out.open("C:\\Work\\tests\\LS_GEN9BT_rc4.cpp");
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=9;\n";

	out << "const uint32_t maxN=1<<N;\n";

	out << "const uint32_t AllN= 511;\n";

	out << "uint32_t LS[N*N];\n";

	out << "uint32_t CR[N*N];\n";

	out << "uint32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	out << "MD|=LS[0];\n";
	out << "AD|=LS[N-1];\n";

	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };

	vector<int> order9_rc1{ 9, 18, 27, 36, 45, 54, 63, 72, 16, 24, 32, 40, 48, 56, 64, 10, 11, 12, 13, 14, 15, 17, 19, 28, 37, 46, 55, 73, 20, 21, 22, 23, 25, 26, 29, 38, 47, 65, 74, 30, 31, 33, 34, 35, 39, 57, 66, 75, 41, 42, 43, 44, 49, 58, 67, 76, 50, 51, 52, 53, 59, 68, 77, 60, 61, 62, 69, 78, 70, 71, 79, 80 };

	vector<int> order9_rc2{ 10, 20, 30, 50, 60, 70, 80, 11, 29, 38, 47, 56, 65, 74, 16, 24, 32, 48, 64, 72, 9, 12, 13, 14, 15, 17, 21, 39, 57, 66, 75, 23, 41, 59, 68, 77, 63, 67, 69, 71, 33, 42, 51, 78, 73, 76, 79, 22, 31, 49, 58, 18, 19, 25, 26, 27, 28, 34, 35, 36, 45, 54, 37, 43, 44, 46, 55, 52, 53, 61, 62 };

	gen_inner_cycle_9BT_rc4(0, order9, out);
	out << "}\n";

	out.close();

}


void gen_inner_cycle_9BT_rc5(int N, int i, vector<int> &order, ofstream &out) {
	int t = order[i];
	//int N = 9;
	vector<int> KN_LS(N*N);
	for (auto j = 0; j < N; j++) {
		KN_LS[j] = 1;
	}
	for (auto j = 0; j <= i; j++) {
		KN_LS[order[j]] = 1;
	}
	bool fullR = false;
	bool fullC = false;
	bool fullMD = false;
	bool fullAD = false;

	int nR = 0;
	int nC = 0;
	int nMD = 0;
	int nAD = 0;
	bool has9 = false;
	for (auto j = 0; j < N; j++) {
		nR += KN_LS[(t / N)*N + j];
		nC += KN_LS[j*N + t % N];
		if (t%N == t / N) {
			nMD += KN_LS[j*N + j];
		}
		if ((t%N) + (t / N) == N - 1) {
			nAD += KN_LS[j*N + (N - j - 1)];
		}
	}

	if ((nR == N) || (nC == N) || (nMD == N) || (nAD == N)) {
		for (auto u = 0; u < N*N; u++) {
			cout << KN_LS[u] << " ";
			if ((u + 1) % N == 0) cout << endl;
		}
		has9 = true;
		if (nR == N) {
			fullR = true;
		}
		if (nC == N) {
			fullC = true;
		}
		if (nMD == N) {
			fullMD = true;
		}
		if (nAD == N) {
			fullAD = true;
		}
		cout << "Has 9 @" << i << " (" << order[i] << ")" << endl;
	}

	string s = "";
	for (auto j = 0; j < i; j++) {
		s += "\t";
	}

	if (has9 == false) {
		out << "for (";
	}


	out << "CR[" << t << "] = AllN ^ (Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (t % N == t / N) out << "| MD ";
	if (t % N == (N - (t / N) - 1)) out << "| AD";
	out << ");";
	if (has9) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=0){\n";

		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
		//		out << s << "if (LS[" << t << "] <MaxN){\n";

	}
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	//	out << "LS[" << t << "] = (!CR[" << t << "] & (CR[" << t << "] + 1));\n";
	// for (CR[t] = Rows[t]|Columns[t]; CR[t]!=AllN; CR[t]=CR[t]|(CR[t]+1){
	//LS[t]=(~CR[t] &(CR[t]+1);

	//L[44] = CR[44] ^ AllN; 
	//for (LS[44] = (L[44])&(-L[44]); LS[44] > 0; L[44] ^= LS[44], LS[44] = (L[44])&(-L[44])) {
	else {
		out << " CR[" << t << "] != 0; CR[" << t << "] = (CR[" << t << "]&(CR[" << t << "] - 1))){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
		//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
		//out << s << "\t\tcontinue;\n\n";
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

	if (i < order.size() - 1) {
		gen_inner_cycle_9BT_rc5(N, i + 1, order, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	if (fullR == false) out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	out << "}\n";
}





void gen_code9BT_rc5(int N, vector<int> order9, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	//out << "int32_t L[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	out << "MD|=LS[0];\n";
	out << "AD|=LS[N-1];\n";

	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_9BT_rc5(N, 0, order9, out);
	out << "}\n";

	out.close();

}





int norm1(int R, int C, int MD, int AD, int N) {
	if ((R == N - 1) || (C == N - 1) || (MD == N - 1) || (AD == N - 1)) {
		return N*N;
	}
	return R + C + MD + AD;
}

int norm2(int R, int C, int MD, int AD, int N) {
	if ((R == N - 1) || (C == N - 1) || (MD == N - 1) || (AD == N - 1)) {
		return N*N;
	}
	int res = R;
	if (C > res) res = C;
	if (MD > res) res = MD;
	if (AD > res) res = AD;
	return res;
}


int norm3(int R, int C, int MD, int AD, int N) {
	if ((R == N - 1) || (C == N - 1) || (MD == N - 1) || (AD == N - 1)) {
		return N*N;
	}
	int res = sqrt(R*R + C*C + MD*MD + AD*AD);
	return res;
}

void compute_lims(int N, vector<int> LS, vector<int> &LR, vector<int> &LC, vector<int> &LMD, vector<int> &LAD) {

	for (auto i = 0; i < N*N; i++) {
		LR[i] = 0;
		LC[i] = 0;
		LMD[i] = 0;
		LAD[i] = 0;
	}
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			if (LS[i*N + j] != 1) {
				//for each element compute number of restrictions imposed on it
				for (auto u = 0; u < N; u++) {
					//column
					if (LS[u*N + j] == 1) { LC[i*N + j]++; }
					//row
					if (LS[i*N + u] == 1) { LR[i*N + j]++; }
				}

				if (i == j) {
					for (auto u = 0; u < N; u++) {
						if (LS[u*N + u] == 1) { LMD[i*N + j]++; }
					}
				}
				if (i + j == N - 1) {
					for (auto u = 0; u < N; u++) {
						if (LS[u*N + (N - u - 1)] == 1) { LAD[i*N + j]++; }
					}
				}
			}
		}
	}

}

void compute_lims_MASK(int N,vector<int> &LSMASK, vector<int> LS, vector<int> &LR, vector<int> &LC, vector<int> &LMD, vector<int> &LAD) {

	for (auto i = 0; i < N*N; i++) {
		LR[i] = 0;
		LC[i] = 0;
		LMD[i] = 0;
		LAD[i] = 0;
	}

	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			if (LSMASK[i*N + j] > 0){
				if (LS[i*N + j] != 1) {
					//for each element compute number of restrictions imposed on it
					for (auto u = 0; u < N; u++) {
						//column
						if ((LS[u*N + j] == 1) && (LSMASK[u*N + j]>0)) { LC[i*N + j]++; }
						//row
						if ((LS[i*N + u] == 1)&&(LSMASK[u*N+j]>0)) { LR[i*N + j]++; }
					}

					if (i == j) {
						for (auto u = 0; u < N; u++) {
							if ((LS[u*N + u] == 1)&&(LSMASK[u*N+u]>0)) { LMD[i*N + j]++; }
						}
					}
					if (i + j == N - 1) {
						for (auto u = 0; u < N; u++) {
							if ((LS[u*N + (N - u - 1)] == 1)&&(LSMASK[u*N + (N - u - 1)]>0)) { LAD[i*N + j]++; }
						}
					}
				}
			}
		}
	}
	//end
}

int NORM(int R, int C, int MD, int AD, int N, int NORM_NUMBER) {
	int res = 0;
	switch (NORM_NUMBER)
	{
	case 1:res = norm1(R, C, MD, AD, N); break;
	case 2:res = norm2(R, C, MD, AD, N); break;
	case 3:res = norm3(R, C, MD, AD, N); break;
	default:
		break;
	}
	return res;
}
vector<int> analyze9(int N, int NORM_N) {
	vector<int> LS(N*N);
	vector<int> order(N*N);
	for (auto i = 0; i < N; i++) {
		LS[i] = 1;
		order[i] = 0;
	}

	for (auto i = N; i < N*N; i++) {
		LS[i] = -1;
		order[i] = -1;
	}

	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMA(N*N);



	int k = -1;
	int cnt = N;

	/*

	cout << "40 10 16 64 70 20 24 56 60 30 ";
	LS[40] = 1;
	order[N   ] = 40;
	LS[10] = 1;
	order[N + 1] = 10;
	LS[16] = 1;
	order[N + 2] = 16;
	LS[64] = 1;
	order[N + 3] = 64;
	LS[70] = 1;
	order[N + 4] = 70;
	LS[20] = 1;
	order[N + 5] = 20;
	LS[24] = 1;
	order[N + 6] = 25;
	LS[56] = 1;
	order[N + 7] = 56;
	LS[60] = 1;
	order[N + 8] = 60;
	LS[30] = 1;
	order[N + 9] = 30;

	cnt += 10;
	*/
	while (cnt<N*N) {

		vector<int> PR(N*N);
		for (int i = 0; i < N*N; i++) {
			if (LS[i] == -1) {
				LS[i] = 1;

				compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);

				int tmp = 0;

				for (auto j = N; j < N*N; j++) {
					tmp += NORM(LimsR[j], LimsC[j], LimsMD[j], LimsAD[j], N, NORM_N);
				}
				PR[i] = tmp;

				LS[i] = -1;
			}


		}

		compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);


		for (auto i = 0; i < N*N; i++) {
			LIMA[i] = NORM(LimsR[i], LimsC[i], LimsMD[i], LimsAD[i], N, NORM_N);
		}

		/*int tmp_rec = PR[0];
		k = 0;
		vector<int> uf;
		for (auto i = 1; i < N*N; i++) {
		if (PR[i] > tmp_rec) {
		//if (PR[i] > tmp_rec) {
		tmp_rec = PR[i];
		k = i;
		}
		}
		for (auto i = 0; i < N*N; i++) {
		if (PR[i] == tmp_rec) {
		uf.push_back(i);
		}
		}
		int tmp_rec2 = LIMA[uf[0]];
		k = uf[0];
		for (auto i = 0; i < uf.size(); i++) {
		if (LIMA[uf[i]]>tmp_rec2) {
		tmp_rec2 = LIMA[uf[i]];
		k = uf[i];
		}
		}


		*/
		int tmp_rec = LIMA[0];
		k = 0;
		vector<int> uf;
		for (auto i = 1; i < N*N; i++) {
			if (LIMA[i] > tmp_rec) {
				//if (PR[i] > tmp_rec) {
				tmp_rec = LIMA[i];
				k = i;
			}
		}
		/*
		for (auto i = 0; i < N*N; i++) {
		if (LIMA[i] == tmp_rec) {
		uf.push_back(i);
		}
		}
		int tmp_rec2 = PR[uf[0]];
		k = uf[0];
		for (auto i = 0; i < uf.size(); i++) {
		if (PR[uf[i]]>tmp_rec2) {
		tmp_rec2 = PR[uf[i]];
		k = uf[i];
		}
		}
		*/
		LS[k] = 1;
		order[cnt] = k;
		cout << k << " ";
		if (order[cnt - 1] == 79) {
			cout << "LIMITS FOR " << k << endl;
			vector<int> r{ 34,43,52 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 31) {
			cout << "LIMITS FOR 31" << endl;
			vector<int> r{ 27, 28, 29, 33,34,35,49 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 49) {
			cout << "LIMITS FOR 49" << endl;
			vector<int> r{ 45,46,47,51,52,53 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 27) {
			cout << "LIMITS FOR 27" << endl;
			vector<int> r{ 28,29,33,34,35,36,45 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 28) {
			cout << "LIMITS FOR 28" << endl;
			vector<int> r{ 29,33,34,35,37,46 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 29) {
			cout << "LIMITS FOR 29" << endl;
			vector<int> r{ 33,34,35,38,47 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 33) {
			cout << "LIMITS FOR 33" << endl;
			vector<int> r{ 34,35, 42, 51 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 34) {
			cout << "LIMITS FOR 34 " << endl;
			vector<int> r{ 35,43,52 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		if (order[cnt - 1] == 35) {
			cout << "LIMITS FOR 35" << endl;
			vector<int> r{ 44,53 };
			for (int i = 0; i < r.size(); i++) {
				cout << "Lims " << r[i] << " : sum =" << LIMA[r[i]] << ", R =  " << LimsR[r[i]] << ", C = " << LimsC[r[i]] << ", MD = " << LimsMD[r[i]] << ", AD = " << LimsAD[r[i]] << endl;
			}
			cout << endl;
		}
		cnt++;
	}
	cout << endl << endl;
	for (auto i = 0; i < N; i++) {
		cout << "-1";
		if (i < 10) {
			cout << "(0";
		}
		else {
			cout << "(" << i;
		}
		cout << i << ") ";
	}
	cout << endl;

	for (auto i = 1; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			for (auto u = 0; u < N*N; u++) {
				if (order[u] == i*N + j) {
					if ((u - N + 1) > 9) {
						cout << u - N + 1;
					}
					else {
						cout << "0" << u - N + 1;
					}
					if (i*N + j > 9) {
						cout << "(" << i*N + j << ") ";
					}
					else {
						cout << "(0" << i*N + j << ") ";
					}

				}
			}
		}
		cout << endl;
	}


	return order;
}




void gen_inner_cycle_BT_rc6(int N, int i, vector<int> &order, vector<int> & status, ofstream &out) {
	int t = order[i];
	//int N = 9;
	vector<int> KN_LS(N*N);
	for (auto j = 0; j < N; j++) {
		KN_LS[j] = 1;
	}
	for (auto j = 0; j <= i; j++) {
		KN_LS[order[j]] = 1;
	}
	bool fullR = false;
	bool fullC = false;
	bool fullMD = false;
	bool fullAD = false;

	int nR = 0;
	int nC = 0;
	int nMD = 0;
	int nAD = 0;
	bool has9 = false;
	for (auto j = 0; j < N; j++) {
		nR += KN_LS[(t / N)*N + j];
		nC += KN_LS[j*N + t % N];
		if (t%N == t / N) {
			nMD += KN_LS[j*N + j];
		}
		if ((t%N) + (t / N) == N - 1) {
			nAD += KN_LS[j*N + (N - j - 1)];
		}
	}

	if ((nR == N) || (nC == N) || (nMD == N) || (nAD == N)) {
		/*	for (auto u = 0; u < N*N; u++) {
		cout << KN_LS[u] << " ";
		if ((u + 1) % N == 0) cout << endl;
		}*/
		has9 = true;
		if (nR == N) {
			fullR = true;
		}
		if (nC == N) {
			fullC = true;
		}
		if (nMD == N) {
			fullMD = true;
		}
		if (nAD == N) {
			fullAD = true;
		}
		//	cout << "Has 9 @" << i << " (" << order[i] << ")" << endl;
	}
	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMMAX(N*N);
	vector<int> LIMSUM(N*N);
	vector<int> v8;
	vector<int> v7;
	compute_lims(N, KN_LS, LimsR, LimsC, LimsMD, LimsAD);
	for (auto j = 0; j < N*N; j++) {
		LIMMAX[j] = NORM(LimsR[j], LimsC[j], LimsMD[j], LimsAD[j], N, 2);
		LIMSUM[j] = NORM(LimsR[j], LimsC[j], LimsMD[j], LimsAD[j], N, 1);
		if ((LIMMAX[j] == N - 2) && (LIMSUM[j] >= N + 2) && (status[j] == 0)) {
			v7.push_back(j);
		}
	}
	/*
	if (v7.size() != 0) {
	cout << "@" << t << "  \nv7 : ";
	for (auto u = 0; u < v7.size(); u++) cout << v7[u] << " ";
	cout << endl;
	}
	for (int u = i + 1; ((u < i + 3)&(u < order.size())); u++) {
	for (auto v = 0; v < v7.size(); v++) {
	if (v7[v] == order[u]) {
	v7.erase(v7.begin() + v);
	break;
	}
	}
	}*/

	out << "L[" << t << "] = Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (t % N == t / N) out << "| MD ";
	if (t % N == (N - (t / N) - 1)) out << "| AD";
	out << ";\n";

	if (has9 == false) {
		out << "for (";
	}
	out << "CR[" << t << "] = AllN ^ L[" << t << "];";
	if (has9) {
		out << "\n";
		out << "if (CR[" << t << "] !=0){\n";

		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
		//		out << s << "if (LS[" << t << "] <MaxN){\n";

	}
	//	for (LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN)); LS[32]>0; CR[32] ^= LS[32], LS[32] = (CR[32] ^ AllN)&(-(CR[32] ^ AllN))) {

	//	out << "LS[" << t << "] = (!CR[" << t << "] & (CR[" << t << "] + 1));\n";
	// for (CR[t] = Rows[t]|Columns[t]; CR[t]!=AllN; CR[t]=CR[t]|(CR[t]+1){
	//LS[t]=(~CR[t] &(CR[t]+1);

	//L[44] = CR[44] ^ AllN; 
	//for (LS[44] = (L[44])&(-L[44]); LS[44] > 0; L[44] ^= LS[44], LS[44] = (L[44])&(-L[44])) {
	else {
		out << " CR[" << t << "] != 0; CR[" << t << "] = (CR[" << t << "]&(CR[" << t << "] - 1))){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
		//out << s << "\t if ((CR[" << t << "] & LS[" << t << "])!=0)\n";
		//out << s << "\t\tcontinue;\n\n";
	}
	if (fullR == false) out << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";

	//out << "CNT[" << t << "]++;\n";

	/*if (v7.size() != 0) {
	for (auto u = 0; u < v7.size(); u++) {
	out << "L[" << v7[u] << "] = Rows[" << v7[u] / N << "] | Columns[" << v7[u] % N << "]";
	if (v7[u] % N == v7[u] / N) out << "| MD ";
	if (v7[u] % N == (N - (v7[u] / N) - 1)) out << "| AD";
	out << ";\n";
	status[v7[u]]++;
	}
	out << "if (";
	for (auto u = 0; u < v7.size(); u++) {
	out << "(L[" << v7[u] << "] == AllN)";
	if (u < v7.size() - 1)out << "||";
	}
	out << ") goto loop" << t << "end;\n";
	}
	*/
	if (i < order.size() - 1) {
		gen_inner_cycle_BT_rc6(N, i + 1, order, status, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	if (v7.size() != 0)
	{
		out << "\nloop" << t << "end:\n";
	}
	if (fullR == false) out << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";

	out << "}\n";
}





void gen_codeBT_rc6(int N, vector<int> order9, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	out << "int32_t L[N*N];\n";

	out << "int32_t CNT[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N], MD, AD;\n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	out << "MD=0;\n";

	out << "AD=0;\n";

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	out << "MD|=LS[0];\n";
	out << "AD|=LS[N-1];\n";

	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
	vector<int> status(N*N);
	gen_inner_cycle_BT_rc6(N, 0, order9, status, out);
	out << "}\n";

	out.close();
}



void gen_inner_cycle_BT_rc7(int N, bool diag, int i, vector<int> order, vector<int>  has9, vector<vector<int>> bounds, ofstream &out) {
	int t = order[i];

	string s = "";

	bool fullR = (has9[i] == 1);
	bool fullC = (has9[i] == 2);
	bool fullMD = (has9[i] == 3);
	bool fullAD = (has9[i] == 4);

	if (has9[i] == 0) {
		out << "for (";
	}

	out << "CR[" << t << "] = AllN ^ (Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (diag == true)
	{
		if (t % N == t / N) out << "| MD ";
		if (t % N == (N - (t / N) - 1)) out << "| AD";
	}
	out << ");";
	if (has9[i]>0) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=0){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
	}
	else {
		out << " CR[" << t << "] != 0; CR[" << t << "] = (CR[" << t << "]&(CR[" << t << "] - 1))){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";
	}
	if (bounds[i].size() != 0) {
		for (auto j = 0; j < bounds[i].size(); j++) {
			out << "L[" << bounds[i][j] << "] = Rows[" << bounds[i][j] / N << "] | Columns[" << bounds[i][j] % N << "]";
			if (diag == true)
			{
				if (bounds[i][j] % N == bounds[i][j] / N) out << "| MD ";
				if (bounds[i][j] % N == (N - (bounds[i][j] / N) - 1)) out << "| AD";
			}
			out << ";\n";
		}
		out << "if (";
		for (auto j = 0; j < bounds[i].size(); j++) {
			out << "(L[" << bounds[i][j] << "] == AllN)";
			if (j < bounds[i].size() - 1) { out << " || "; }
		}
		out << ") goto loop" << t << "end;\n";
	}

	if (i < order.size() - 1) {
		gen_inner_cycle_BT_rc7(N, diag, i + 1, order, has9, bounds, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}
	if (bounds[i].size() != 0) {
		out << "loop" << t << "end:\n";
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	}
	out << "}\n";
}





void gen_codeBT_rc7(int N, bool diag, vector<int> order, vector<int> hasN, vector<vector<int>> bounds, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	out << "int32_t L[N*N];\n";

	out << "int32_t CNT[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N]";
	if (diag == true) {
		out << ", MD, AD";
	}
	out << "; \n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	if (diag == true) {
		out << "MD=0;\n";

		out << "AD=0;\n";
	}

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	if (diag == false) {
		out << "for (int j=1;j<N;j++){\n";
		out << "LS[j*N] = 1<<j;\n	Rows[j]|=LS[j*N];\n";
		out << "\t}\n";
		out << "Columns[0]=AllN;\n";
	}

	if (diag == true) {
		out << "MD|=LS[0];\n";
		out << "AD|=LS[N-1];\n";
	}
	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_BT_rc7(N, diag, 0, order, hasN, bounds, out);
	out << "}\n";

	out.close();
}

void analyze(int N, int NORM_N, int mode, bool diag, vector<int> MASK, vector<int> & order, int nstart, int nend, int minthreshold, int maxthreshold, int overhead, vector<vector<int>> &sections, vector<int>& has9) {
	vector<int> LS(N*N);
	int cnt = 0;
	int mcount = 0;
	int k = -1;
	order.clear();
	sections.clear();
	has9.clear();
	vector<int> emptyvec;
	vector<int> status(N*N);

	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMA(N*N);

	for (auto i = 0; i < N*N; i++) {
		LS[i] = MASK[i];
		if (LS[i] == 1) {
			status[i]++;
			cnt++;
			mcount++;
			order.push_back(i);
			sections.push_back(emptyvec);
			has9.push_back(0);
		}
	}

	while (cnt<N*N) {
		compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);
		if (diag == true) {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], LimsMD[i], LimsAD[i], N, NORM_N);
			}
		}
		else {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], 0, 0, N, NORM_N);
			}
		}
		int tmp_rec = LIMA[0];
		k = 0;
		for (auto i = 1; i < N*N; i++) {
			if (LIMA[i] > tmp_rec) {
				tmp_rec = LIMA[i];
				k = i;
			}
		}
		if (LIMA[k] == N*N) {
			if (LimsR[k] == N - 1) has9.push_back(1);
			else if (LimsC[k] == N - 1) has9.push_back(2);
			else if (LimsMD[k] == N - 1) has9.push_back(3);
			else if (LimsAD[k] == N - 1) has9.push_back(4);
		}
		else {
			has9.push_back(0);
		}
		LS[k] = 1;

		order.push_back(k);
		cnt++;
	}
	cout << endl << endl;

	for (auto i = 0; i < N*N; i++) {
		LS[i] = MASK[i];
		status[i] = MASK[i];
	}

	for (int i = mcount; i < order.size() - 1; i++) {
		LS[order[i]] = 1;
		status[order[i]]++;
		compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);
		if (diag == true) {
			for (auto j = 0; j< N*N; j++) {
				LIMA[j] = NORM(LimsR[j], LimsC[j], LimsMD[j], LimsAD[j], N, NORM_N);
			}
		}
		else {
			for (auto j = 0; j < N*N; j++) {
				LIMA[j] = NORM(LimsR[j], LimsC[j], 0, 0, N, NORM_N);
			}
		}
		if ((i >= mcount + nstart) && (i <= mcount + nend)) {
			vector<int> tmp;
			for (int j = 0; j < N*N; j++) {
				if ((LIMA[j] >= minthreshold) && (LIMA[j] <= maxthreshold) && (status[j] == 0)) {
					if (mode == 0) {
						if ((j%N == order[i - 1] % N) || (j / N == order[i - 1] / N)) {
						//if ((j%N == order[i] % N) || (j / N == order[i] / N)) {
							tmp.push_back(j);
						}
					}
					if (mode == 1) {
						tmp.push_back(j);
						status[j]++;
					}

				}
			}
			sections.push_back(tmp);
		}
		else {
			sections.push_back(emptyvec);
		}


	}
	sections.push_back(emptyvec);




	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			if (MASK[i*N + j] == 1) {
				cout << " -1 ";
			}
			else
				for (auto u = 0; u < N*N; u++) {
					if (order[u] == i*N + j) {
						if ((u - N + 1) > 9) {
							cout << u - N + 1;
						}
						else {
							cout << "0" << u - mcount;
						}
						if (i*N + j > 9) {
							cout << "(" << i*N + j << ") ";
						}
						else {
							cout << "(0" << i*N + j << ") ";
						}

					}
				}
		}
		cout << endl;
	}
	for (int i = 0; i < mcount; i++) {
		order.erase(order.begin());
	}
	/*for (int i = 0; i < mcount; i++) {
		order.erase(order.begin());
		sections.erase(sections.begin());
		has9.erase(has9.begin());
	}*/
	/*for (int i = 0; i < sections.size() - overhead; i++) {
		if (sections[i].size() != 0) {
			for (int j = 0; j < sections[i].size(); j++) {
				for (auto u = 0; u < overhead; u++) {
					if (sections[i].size()>0) if (j<sections[i].size()) {

						if (sections[i][j] == order[i + u]) {
							sections[i].erase(sections[i].begin() + j);
						}
					}
				}
			}
		}
	}

	cout << endl << "Constraints:\n";
	for (int i = 0; i < order.size(); i++) {
		if (sections[i].size() != 0) {
			cout << "@" << order[i] << "(" << i << "): ";
			for (auto j = 0; j < sections[i].size(); j++) {
				cout << sections[i][j] << " ";
			}
			cout << endl;
		}
	}*/


}

void gen_inner_cycle_analyze_rc1(int N, bool diag, vector<int> order, int ind, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, ofstream &out) {
	int t = order[ind];
	cout << ind << endl;
	string s = "";


	bool fullR = (limr[ind] == N-1);
	bool fullC = (limc[ind] == N-1);
	bool fullMD = (limmd[ind] == N-1);
	bool fullAD = (limad[ind] == N-1);

	bool has9 = fullR || fullC || fullMD || fullAD;
	if (!has9) {
		out << "for (";
	}

	out << "CR[" << t << "] = AllN ^ (Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (diag == true)
	{
		if (t % N == t / N) out << "| MD ";
		if (t % N == (N - (t / N) - 1)) out << "| AD";
	}
	out << ");";
	if (has9) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=0){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
	}
	else {
		out << " CR[" << t << "] != 0; CR[" << t << "] = (CR[" << t << "]&(CR[" << t << "] - 1))){\n";
		out << "LS[" << t << "] = (CR[" << t << "] & (-CR[" << t << "]));\n";
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";
	}
	
	if (ind<order.size()-1) {
		gen_inner_cycle_analyze_rc1(N, diag, order, ind + 1, limr, limc, limmd, limad, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}
	
	if (fullR == false) out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	}
	out << "}\n";
}


void gen_codeBT_analyze_rc1(int N, bool diag, vector<int> order, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	out << "int32_t L[N*N];\n";

	out << "int32_t CNT[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N]";
	if (diag == true) {
		out << ", MD, AD";
	}
	out << "; \n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	if (diag == true) {
		out << "MD=0;\n";

		out << "AD=0;\n";
	}

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	if (diag == false) {
		out << "for (int j=1;j<N;j++){\n";
		out << "LS[j*N] = 1<<j;\n	Rows[j]|=LS[j*N];\n";
		out << "\t}\n";
		out << "Columns[0]=AllN;\n";
	}

	if (diag == true) {
		out << "MD|=LS[0];\n";
		out << "AD|=LS[N-1];\n";
	}
	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_analyze_rc1(N, diag, order, 0, limr, limc, limmd, limad, out);
	out << "}\n";

	out.close();
}


void analyze_rc1(int N, int NORM_N, bool diag, vector<int> MASK, string filename) {
	vector<int> LS(N*N);
	vector<int> order;
	int cnt = 0;
	int mcount = 0;
	int k = -1;
	order.clear();
	vector<int> emptyvec;
	vector<int> status(N*N);

	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMA(N*N);


	vector<int> lim_r_out;
	vector<int> lim_c_out;
	vector<int> lim_md_out;
	vector<int> lim_ad_out;
	for (auto i = 0; i < N*N; i++) {
		LS[i] = MASK[i];
		if (LS[i] == 1) {
			status[i]++;
			cnt++;
			mcount++;					
		}
	}
	

	
	while (cnt<N*N) {
		compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);
		if (diag == true) {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], LimsMD[i], LimsAD[i], N, NORM_N);
			}
		}
		else {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], 0, 0, N, NORM_N);
			}
		}
		int tmp_rec = LIMA[0];
		k = 0;
		for (auto i = 1; i < N*N; i++) {
			if (LIMA[i] > tmp_rec) {
				tmp_rec = LIMA[i];
				k = i;
			}
		}	
		LS[k] = 1;

		lim_r_out.push_back(LimsR[k]);
		lim_c_out.push_back(LimsC[k]);
		lim_md_out.push_back(LimsMD[k]);
		lim_ad_out.push_back(LimsAD[k]);
		order.push_back(k);
		cnt++;
	}
	gen_codeBT_analyze_rc1(N,diag,order, lim_r_out, lim_c_out, lim_md_out, lim_ad_out, filename);
}


void gen_inner_cycle_analyze_rc2(int N, bool diag, bool BMI, vector<int> order, int ind, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, ofstream &out) {
	int t = order[ind];
	cout << ind << endl;
	string s = "";


	bool fullR = (limr[ind] == N - 1);
	bool fullC = (limc[ind] == N - 1);
	bool fullMD = (limmd[ind] == N - 1);
	bool fullAD = (limad[ind] == N - 1);

	bool has9 = fullR || fullC || fullMD || fullAD;
	if (!has9) {
		out << "for (";
	}

	out << "CR[" << t << "] = AllN ^ (Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (diag == true)
	{
		if (t % N == t / N) out << "| MD ";
		if (t % N == (N - (t / N) - 1)) out << "| AD";
	}
	out << ");";
	if (has9) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=0){\n";
		if (BMI == true) {
			out << "LS[" << t << "] = _blsi_u64(CR[" << t << "]);\n";
		}
		else {
			out << "LS[" << t << "] = (-CR[" << t << "]) & CR["<<t<<"];\n";
		}
	}
	else {
		if (BMI==true){
			out << "CR[" << t << "] != 0; CR[" << t << "] = _blsr_u64(CR[" << t << "])){\n";			
			out << "LS[" << t << "] = _blsi_u64 (CR[" << t << "]);\n";
		}
		else {			
			out << "CR[" << t << "] != 0; CR[" << t << "] = (CR[" << t << "]-1) & CR[" << t << "]) {\n";
			out << "LS[" << t << "] = (-CR[" << t << "]) & CR[" << t << "];\n";
		}
	}
	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";
	}

	if (ind<order.size() - 1) {
		gen_inner_cycle_analyze_rc2(N, diag, BMI, order, ind + 1, limr, limc, limmd, limad, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		//out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\tcout << \"Squares found: \" << SquaresCnt << endl;\n";		
		out << "}\n";
	}

	if (fullR == false) out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
	if (diag == true)
	{
		if (fullMD == false) if (t % N == t / N) out << "MD ^= LS[" << t << "];\n";
		if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD ^=LS[" << t << "];\n";
	}
	out << "}\n";
}


void gen_codeBT_analyze_rc2(int N, bool diag, bool BMI, vector<int> order, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " #include \"immintrin.h\"\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	out << "int32_t L[N*N];\n";

	out << "int32_t CNT[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N]";
	if (diag == true) {
		out << ", MD, AD";
	}
	out << "; \n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	//	out << "__int64 _StartProf, _StopProf;\n";

//	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	//out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	//out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	if (diag == true) {
		out << "MD=0;\n";

		out << "AD=0;\n";
	}

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	if (diag == false) {
		out << "for (int j=1;j<N;j++){\n";
		out << "LS[j*N] = 1<<j;\n	Rows[j]|=LS[j*N];\n";
		out << "\t}\n";
		out << "Columns[0]=AllN;\n";
	}

	if (diag == true) {
		out << "MD|=LS[0];\n";
		out << "AD|=LS[N-1];\n";
	}
	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	//out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_analyze_rc2(N, diag, BMI, order, 0, limr, limc, limmd, limad, out);
	out << "}\n";

	out.close();
}


void analyze_rc2(int N, int NORM_N, bool diag, bool BMI, vector<int> MASK, string filename) {
	vector<int> LS(N*N);
	vector<int> order;
	int cnt = 0;
	int mcount = 0;
	int k = -1;
	order.clear();
	vector<int> emptyvec;
	vector<int> status(N*N);

	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMA(N*N);


	vector<int> lim_r_out;
	vector<int> lim_c_out;
	vector<int> lim_md_out;
	vector<int> lim_ad_out;
	for (auto i = 0; i < N*N; i++) {
		LS[i] = MASK[i];
		if (LS[i] == 1) {
			status[i]++;
			cnt++;
			mcount++;
		}
	}



	while (cnt<N*N) {
		compute_lims(N, LS, LimsR, LimsC, LimsMD, LimsAD);
		if (diag == true) {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], LimsMD[i], LimsAD[i], N, NORM_N);
			}
		}
		else {
			for (auto i = 0; i < N*N; i++) {
				LIMA[i] = NORM(LimsR[i], LimsC[i], 0, 0, N, NORM_N);
			}
		}
		int tmp_rec = LIMA[0];
		k = 0;
		for (auto i = 1; i < N*N; i++) {
			if (LIMA[i] > tmp_rec) {
				tmp_rec = LIMA[i];
				k = i;
			}
		}
		//dirty hack
		for (auto i = 0; i < N; i++) {
			if (LS[i*N + (N - 1 - i)] != 1) {
				k = i * N + (N - 1 - i);
			}
		}
		//dirty hack
		LS[k] = 1;

		lim_r_out.push_back(LimsR[k]);
		lim_c_out.push_back(LimsC[k]);
		lim_md_out.push_back(LimsMD[k]);
		lim_ad_out.push_back(LimsAD[k]);
		order.push_back(k);
		cnt++;
	}
	gen_codeBT_analyze_rc2(N, diag, BMI, order, lim_r_out, lim_c_out, lim_md_out, lim_ad_out, filename);
}







void gen_inner_cycle_sym_analyze_rc2(int N, bool diag, vector<int> order, int ind, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, ofstream &out) {
	int t = order[ind];
	cout << ind << endl;
	string s = "";
	int t_star = N*(t / N) + (N - 1 - t%N);

	bool fullR = (limr[ind] == N - 1);
	bool fullC = (limc[ind] == N - 1);
	bool fullMD = (limmd[ind] == N - 1);
	bool fullAD = (limad[ind] == N - 1);

	bool has9 = fullR || fullC || fullMD || fullAD;
	if (!has9) {
		out << "for (";
	}

	out << "CR[" << t << "] = AllN ^ (Rows[" << t / N << "] | Columns[" << t % N << "]";
	if (diag == true)
	{
		if (t % N == t / N) out << "| MD ";
		if (t % N == (N - (t / N) - 1)) out << "| AD";
	}
	out << ");";
	if (has9) {
		out << "\n";
		out << s << "if (CR[" << t << "] !=0){\n";
		out << "LS[" << t << "] = _blsi_u64(CR[" << t << "]);\n";
	}
	else {
		out << " CR[" << t << "] != 0; CR[" << t << "] = _blsr_u64(CR[" << t << "])){\n";
		out << "LS[" << t << "] = _blsi_u64 (CR[" << t << "]);\n";

		out << "LS[" << t_star << "] = 1 << (N-1-_tzcnt_u64(LS[" << t << "]));\n";
		//out << "LS[" << t_star << "] = table[LS[" << t << "]];\n";
	}
	if (fullR == false) {
		out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";	
		out << s << "Rows[" << t / N << "] |= LS[" << t_star << "];\n";
	}
	if (fullC == false) {
		out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";		
		out << s << "Columns[" << t_star % N << "] |= LS[" << t_star<< "];\n";
	}

	if (diag == true)
	{
		if (fullMD == false) {
			if (t % N == t / N) {
				out << "MD |= LS[" << t << "];\n";
				out << "AD |= LS[" << t_star << "];\n";
			}
		}
		if (fullAD == false) {
			if (t % N == (N - (t / N) - 1)) {
				out << "AD |=LS[" << t << "];\n";
				out << "MD |=LS[" << t_star << "];\n";
			}
		}
	}

	if (ind<order.size() - 1) {
		gen_inner_cycle_sym_analyze_rc2(N, diag, order, ind + 1, limr, limc, limmd, limad, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	if (fullR == false) {
		out << s << "Rows[" << t / N << "] ^= LS[" << t << "];\n";
		out << s << "Rows[" << t / N << "] ^= LS[" << t_star<< "];\n";
	}
	if (fullC == false) {
		out << s << "Columns[" << t % N << "] ^= LS[" << t << "];\n";
		out << s << "Columns[" << t_star % N << "] ^= LS[" << t_star << "];\n";
	}
	if (diag == true)
	{
		if (fullMD == false) {
			if (t % N == t / N) {
				out << "MD ^= LS[" << t << "];\n";
				out << "AD ^= LS[" << t_star << "];\n";
			}

		}
		if (fullAD == false) {
			if (t % N == (N - (t / N) - 1)) {
				out << "AD ^=LS[" << t << "];\n";
				out << "MD^=LS[" << t_star << "];\n";
			}
		}
	}
	out << "}\n";
}


void gen_codeBT_sym_analyze_rc2(int N, bool diag, vector<int> order, vector<int>limr, vector<int> limc, vector<int> limmd, vector<int> limad, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " #include \"immintrin.h\"\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint32_t MaxN=1<<N;\n";

	out << "const uint32_t AllN= MaxN-1;\n";




	out << "static const unsigned char table[] = {\n";
	out << "\t0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,\n";
	out << "\t0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,\n";
	out << "\t0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,\n";
	out << "\t0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,\n";
	out << "\t0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,\n";
	out << "\t0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,\n";
	out << "\t0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,\n";
	out << "\t0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,\n";
	out << "\t0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,\n";
	out << "\t0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,\n";
	out << "\t0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,\n";
	out << "\t0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,\n";
	out << "\t0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,\n";
	out << "\t0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,\n";
	out << "\t0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,\n";
	out << "\t0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,\n";
	out << "\t0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,\n";
	out << "\t0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,\n";
	out << "\t0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,\n";
	out << "\t0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,\n";
	out << "\t0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,\n";
	out << "\t0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,\n";
	out << "\t0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,\n";
	out << "\t0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,\n";
	out << "\t0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,\n";
	out << "\t0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,\n";
	out << "\t0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,\n";
	out << "\t0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,\n";
	out << "\t0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,\n";
	out << "\t0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,\n";
	out << "\t0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,\n";
	out << "\t0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,\n";
	out << "};\n";





	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	out << "int32_t LS[N*N];\n";

	out << "int32_t CR[N*N];\n";

	out << "int32_t L[N*N];\n";

	out << "int32_t CNT[N*N];\n";

	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint32_t Columns[N], Rows[N]";
	if (diag == true) {
		out << ", MD, AD";
	}
	out << "; \n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";



	out << "void Reset()\n{\n";

	out << "for (int i = 0; i < N*N; i++)\n\tLS[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\tLS[i]=1<<i;\n";

	out << "for (int i = 0; i < N; i++)\n\t Columns[i]=0;\n";

	out << "for (int i = 0; i < N; i++)\n\t Rows[i]=0;\n";

	if (diag == true) {
		out << "MD=0;\n";

		out << "AD=0;\n";
	}

	out << "for (int j=0;j<N;j++){\n";

	out << "LS[j] = 1<<j;\n	Rows[0]|=LS[j];\nColumns[j]|= LS[j];\n";
	out << "\t}\n";
	if (diag == false) {
		out << "for (int j=1;j<N;j++){\n";
		out << "LS[j*N] = 1<<j;\n	Rows[j]|=LS[j*N];\n";
		out << "\t}\n";
		out << "Columns[0]=AllN;\n";
	}

	if (diag == true) {
		out << "MD|=LS[0];\n";
		out << "AD|=LS[N-1];\n";
	}
	out << "}\n\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_sym_analyze_rc2(N, diag, order, 0, limr, limc, limmd, limad, out);
	out << "}\n";

	out.close();
}


void analyze_sym_rc2(int N, int NORM_N, bool diag, vector<int> MASK, vector<int> LSMASK, string filename) {
	//vector<int> LS(N*N);
	vector<int> LS(N*N);
	vector<int> order;
	int cnt = 0;
	int mcount = 0;
	int k = -1;
	order.clear();
	vector<int> emptyvec;
	vector<int> status(N*N);

	vector<int> LimsR(N*N);
	vector<int> LimsC(N*N);
	vector<int> LimsMD(N*N);
	vector<int> LimsAD(N*N);
	vector<int> LIMA(N*N);


	vector<int> lim_r_out;
	vector<int> lim_c_out;
	vector<int> lim_md_out;
	vector<int> lim_ad_out;
	int LSM_count = 0;
	for (int i = 0; i < N*N; i++) {
		if (LSMASK[i] > 0) { LSM_count++; }
	}


	for (auto i = 0; i < N*N; i++) {
		if (LSMASK[i] > 0) {
			LS[i] = MASK[i];
		}
		else{
			LS[i] = 0;
		}
		if (LS[i] == 1) {
			status[i]++;
			cnt++;
			mcount++;
		}
	}

	while (cnt < LSM_count) {
		compute_lims_MASK(N, LSMASK, LS, LimsR, LimsC, LimsMD, LimsAD);
		
		if (diag == true) {
			for (auto i = 0; i < N*N; i++) {
				if (LSMASK[i]>0){
					LIMA[i] = NORM(LimsR[i], LimsC[i], LimsMD[i], LimsAD[i], N, NORM_N);
				}
				else {
					LIMA[i] = 0;
				}
			}
		}
		else {
			for (auto i = 0; i < N*N; i++) {
				if (LSMASK[i] > 0) {
					LIMA[i] = NORM(LimsR[i], LimsC[i], 0, 0, N, NORM_N);
				}
				else
				{
					LIMA[i] = 0;
				}
			}
		}
		int tmp_rec = LIMA[0];
		k = 0;
		for (auto i = 0; i < N; i++) {
			for (auto j = 0; j < N; j++) {
				if (LIMA[i*N + j] > tmp_rec) {
					tmp_rec = LIMA[i];
					k = i*N+j;
				}
			}
		}		
		int it = k / N;
		int jt = k % N;
		int k_star = it*N + (N - jt - 1);
		if (k_star < k) { 
			swap(k, k_star); 
		}
		LS[k] = 1;
		cnt++;
		
		LS[k_star] = 1;
		cnt++;

		lim_r_out.push_back(LimsR[k]);
		lim_c_out.push_back(LimsC[k]);
		lim_md_out.push_back(LimsMD[k]);
		lim_ad_out.push_back(LimsAD[k]);
		order.push_back(k);
		
	}
	gen_codeBT_sym_analyze_rc2(N, diag, order, lim_r_out, lim_c_out, lim_md_out, lim_ad_out, filename);
}




void gen_inner_cycle_BT_EXT(int N, int i, vector<int> order, vector<int>  has9, vector<string> LS, vector<string> CR, vector<int> LS_shift, vector<int>CR_shift, vector<string> Rows, vector<int> Rows_shift, vector<string> Cols, vector<int> cols_shift, string MD_s, string AD_s, int MD_shift, int AD_shift, ofstream &out) {
	int t = order[i];
	out << "// cycle " << i << " Cell " << order[i] << endl;
	out << "//print_status();\n //print_status_RCD();\n//print_status_CR();\n\n";
	string s = "";

	bool fullR = (has9[i] == 1);
	bool fullC = (has9[i] == 2);
	bool fullMD = (has9[i] == 3);
	bool fullAD = (has9[i] == 4);

	out << CR[t] << " |= (AllN) << " << CR_shift[t] << ";\n";
	if (has9[i] == 0) {
		out << "for (";
	}

	/*CR[3]|=(AllN)<<18;
	for (CR[3]^=(AllN&((Rows>>18)| (Columns>>36)|(DD>>9)))<<18;// because we simply purge everything that doesnt matter and x ^ 0=x;
	((CR[3]>>18)&AllN)!=0;
	CR[3]&=MAXN &((((CR[3]>>18)&((CR[3]>>18)-1))<<18)
	*/
	out << CR[t] << " ^= (AllN & ((" << Rows[t / N] << " >> " << Rows_shift[t / N] << ") | (" << Cols[t%N] << " >> " << cols_shift[t%N] << ")";
	if (t % N == t / N) out << "| (" << MD_s << " >> " << MD_shift << ")";
	if (t % N == (N - (t / N) - 1)) out << "| (" << AD_s << " >> " << AD_shift << ")";
	out << "))<<"<<CR_shift[t]<<";";

	if (has9[i]>0) {
		out << "\n";
		out << "if (((" << CR[t] << ">>" << CR_shift[t] << ") & AllN) != 0){\n";
		
		
		out << "//cout<<\"CR[" << t << "] =\";\n";
		out << "//print_chint(" << CR[t] << ", " << CR_shift[t] << ", " << CR_shift[t] + N << ");\n";
		out << "//cout<<endl;\n";


		out << LS[t] << " &= (~(AllN << " << LS_shift[t] << ")) & MAXN;\n";
		out << LS[t] << " |= (((" << CR[t] << " >> " << CR_shift[t] << ") & (-(" << CR[t] << " >> " << CR_shift[t] << "))) & AllN) <<" << LS_shift[t] << ";\n";
	}
	else {
		/*CR[3]|=(AllN)<<18;
		for (CR[3]^=(AllN&((Rows>>18)| (Columns>>36)|(DD>>9)))<<18;// because we simply purge everything that doesnt matter and x ^ 0=x;
		((CR[3]>>18)&AllN)!=0;
		CR[3]&=MAXN &((((CR[3]>>18)&((CR[3]>>18)-1))<<18)
		*/
		out << "((" << CR[t] << ">>" << CR_shift[t] << ") & AllN) != 0;";
		out << CR[t] << " ^= (((" << CR[t] << " >> " << CR_shift[t] << ") & (-(" << CR[t] << " >> " << CR_shift[t] << "))) << " << CR_shift[t] << ")){\n";

		out << "//cout<<\"CR[" << t << "] =\";\n";
		out << "//print_chint(" << CR[t] << ", " << CR_shift[t] << ", " << CR_shift[t] + N << ");\n";
		out << "//cout<<endl;\n";

		out << LS[t] << " &= (~(AllN << " << LS_shift[t] << ")) & MAXN;\n";
		out << LS[t] << " |= (((" << CR[t] << " >> " << CR_shift[t] << ") & (-(" << CR[t] << " >> " << CR_shift[t] << "))) & AllN) <<" << LS_shift[t] << ";\n";
	}

	/*	out << Rows[order[i] / N] << " |= ((" << LS[order[i]] << " >> " << LS_shift[order[i]] << ")&AllN) <<" << Rows_shift[order[i] / N] << ";\n";
	out << Cols[order[i] % N] << " |= ((" << LS[order[i]] << " >> " << LS_shift[order[i]] << ")&AllN) <<" << Cols_shift[order[i] % N] << ";\n";
	if (order[i] % N == order[i] / N) out << MD_s << " |= ((" << LS[order[i]] << " >> " << LS_shift[order[i]] << ")&AllN) <<" << MD_shift << ";\n";
	if (order[i] % N == (N - (order[i] / N) - 1)) out << AD_s << " |= ((" << LS[order[i]] << " >> " << LS_shift[order[i]] << ")&AllN) <<" << AD_shift << ";\n";
	*/


	if (fullR == false) {
		out << "//Refreshing Rows[" << t / N << "];\n";
		out << Rows[t / N] << " |= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << Rows_shift[t / N] << ";\n";
	}

	if (fullC == false) {
		out << "//Refreshing Columns[" << t % N << "];\n";
		out << Cols[t % N] << " |= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << cols_shift[t % N] << ";\n";
	}
	if (fullMD == false) {
		
		if (t % N == t / N) {
			out << "//Refreshing MD;\n";
			out << MD_s << " |= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << MD_shift << ";\n";		
		}
	}
	if (fullAD == false) {		
		if (t % N == (N - (t / N) - 1)) {
			out << "//Refreshing AD;\n";
			out << AD_s << " |= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << AD_shift << ";\n";
		}
	}

	if (i < order.size() - 1) {
		gen_inner_cycle_BT_EXT(N, i + 1, order, has9, LS, CR, LS_shift, CR_shift, Rows, Rows_shift, Cols, cols_shift, MD_s, AD_s, MD_shift, AD_shift, out);
	}
	else {
		out << "SquaresCnt++;\n \t if (SquaresCnt % 10000000 == 0) {\n\t\t";
		out << "\tStopProf();\n\tcout << \"Squares found: \" << SquaresCnt << endl;\n";
		out << "\t\tcout << \"Average time: \" << 1.0*ProfResult() / ProcFreq / SquaresCnt * 1000 << \" ms per square\" << endl;\n";
		out << "\t\tcout << \"Squares per second: \" << 1.0 / (1.0 * ProfResult() / ProcFreq / SquaresCnt) << endl << endl;\n";
		out << "}\n";
	}

	if (fullR == false) out << Rows[t / N] << " ^= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << Rows_shift[t / N] << ";\n";

	if (fullC == false)	out << Cols[t % N] << " ^= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << cols_shift[t % N] << ";\n";
	if (fullMD == false) if (t % N == t / N) out << MD_s << " ^= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << MD_shift << ";\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << AD_s << " ^= ((" << LS[t] << " >> " << LS_shift[t] << ")&AllN) <<" << AD_shift << ";\n";

	out << "}\n";
}




int strtoi(string s) {
	int x = atoi(s.c_str());
	return x;
}
string inttostr(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

void gen_codeBT_EXT(int N, int d_size, bool diag, vector<int> order, vector<int> hasN, vector<vector<int>> bounds, string fn) {
	ofstream out;

	out.open(fn);
	out << " #include <iostream>\n";
	out << " #include <chrono>\n";
	out << " #include <vector>\n";
	out << " #include <bitset>\n";
	out << " static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }\n";


	out << "using namespace std;\n";

	out << "const int N=" << N << ";\n";

	out << "const uint64_t MAXN=18446744073709551615;\n";

	out << "const uint64_t AllN= (1<<N)-1;\n";

	//out << "const uint32_t maxINT = UINT32_MAX^AllN;\n";

	int t_size = N*N - N - d_size;
	vector<string> LS(N*N);
	vector<string> CR(N*N);
	vector<string> Rows(N);
	vector<string> Cols(N);
	string AD_s;
	string MD_s;

	vector<int> LS_shift(N*N);
	vector<int> CR_shift(N*N);
	vector<int> Rows_shift(N);
	vector<int> Cols_shift(N);
	int AD_shift;
	int MD_shift;

	int k = 0;
	int i = 0;
	while (i < N*N) {
		out << "uint64_t LS" << k << ";\n";
		out << "uint64_t CR" << k << ";\n";
		int t = 0;
		while ((t < 64 / N) && (i + t<N*N)) {
			LS[i + t] = "LS" + inttostr(k);
			LS_shift[i + t] = t*N;
			CR[i + t] = "CR" + inttostr(k);
			CR_shift[i + t] = t*N;
			t++;
		}
		i += t;
		k++;
	}



	out << "uint64_t SquaresCnt = 0;\n";

	out << "uint64_t Rows;\n";
	out << "uint64_t Columns;\n";
	out << "uint64_t DMIX;\n";


	for (int i = 0; i < 64 / N; i++) {
		Rows[i] = "Rows";
		Rows_shift[i] = i*N;

		Cols[i] = "Columns";
		Cols_shift[i] = i*N;
	}
	int t = 0;

	for (int i = 64 / N; i < N; i++) {
		Rows[i] = "DMIX";
		Rows_shift[i] = t*N;
		t++;
	}
	for (int i = 64 / N; i < N; i++) {
		Cols[i] = "DMIX";
		Cols_shift[i] = t*N;
		t++;
	}
	AD_s = "DMIX";
	AD_shift = t*N;
	t++;
	MD_s = "DMIX";
	MD_shift = t*N;

	if (t > 64 / N) { cout << "we are in trouble\n"; }
	out << "char FileData[11] = { '1','2','0','3','6','5','2','6','4','7',NULL }; \n";

	out << "__int64 ProcFreq = 3.5e9;\n";

	out << "__int64 _StartProf, _StopProf;\n";

	out << "void StartProf()\n {\n\t_StartProf = __rdtsc();\n}\n";

	out << "void StopProf()\n {\n\t_StopProf = __rdtsc();\n}\n";

	out << "__int64 ProfResult()\n {\n\t return _StopProf - _StartProf;\n}\n";


	out << "void Reset()\n{\n";

	for (int i = 0; i < k; i++) {
		out << "LS" << i << " = 0;\n";
		out << "CR" << i << " = 0;\n";
	}
	//			LS3&=(!(AllN<<18))&MAXN;
	//	LS3 |= ((CR[3] >> 18) & (-CR[3] >> 18))&AllN) << 18;
	out << "Rows=0;\nColumns=0;\nDMIX=0;\n";

	for (int i = 0; i < N; i++) {
		out << LS[i] << "|= (1ULL<<" << i << ")<<" << LS_shift[i] << ";\n";
		out << Rows[0] << " |= (1ULL<<" << i << ") <<" << Rows_shift[i / N] << ";\n";
		out << Cols[i] << " |= (1ULL<<" << i << ")<<" << Cols_shift[i] << ";\n";
		if (i == 0) out << MD_s << " |= (1ULL<<(" << i << ")) <<" << MD_shift << ";\n";
		if (i == N - 1) out << AD_s << " |= (1ULL<<" << i << ")<<" << AD_shift << ";\n";

	}

	for (int i = 0; i < d_size; i++) {
		out << LS[order[i]] << "|= (1ULL<<((int)FileData[" << i << "] - 0x30))<<" << LS_shift[order[i]] << ";\n";
		out << Rows[order[i] / N] << " |= (1ULL<<((int)FileData[" << i << "] - 0x30)) <<" << Rows_shift[order[i] / N] << ";\n";
		out << Cols[order[i] % N] << " |= (1ULL<<((int)FileData[" << i << "] - 0x30))<<" << Cols_shift[order[i] % N] << ";\n";
		if (order[i] % N == order[i] / N) out << MD_s << " |= (1ULL<<((int)FileData[" << i << "] - 0x30)) <<" << MD_shift << ";\n";
		if (order[i] % N == (N - (order[i] / N) - 1)) out << AD_s << " |= (1ULL<<((int)FileData[" << i << "] - 0x30))<<" << AD_shift << ";\n";
	}

	/*Rows|= ((LS3>>18)&AllN)<<18;
	Columns |= ((LS3>>18)&AllN)<<36;
	AD|=((LS3>>18)&AllN)<<9;
	*/

	/*	if (fullR == false) out << s << "Rows[" << t / N << "] |= LS[" << t << "];\n";
	if (fullC == false) out << s << "Columns[" << t % N << "] |= LS[" << t << "];\n";
	if (diag == true)
	{
	if (fullMD == false) if (t % N == t / N) out << "MD |= LS[" << t << "];\n";
	if (fullAD == false) if (t % N == (N - (t / N) - 1)) out << "AD |=LS[" << t << "];\n";
	}*/



	out << "}\n\n";

	out << "void print(uint64_t v, int i1, int i2) {\n";
	out << "	bitset<64> x(v);\n";
	out << "	for (int i = i1; i < i2; i++) {\n";
	out << "cout << x[i];\n";
	out << "}\n";
	out << "cout << endl;";
	out << "}\n";

	out << "void print_int(uint64_t v, int i1, int i2) {\n";
	out << "	bitset<64> x(v);\n";
	out << "bool b=false;\n";
	out << "	for (int i = i1; i < i2; i++) \n";
	out << "if (x[i] == 1)	{cout << i - i1; b=true;}	\n";
	out << "if (b==false)	{cout << \"X\";}	\n";
	out << "cout<<\" \";\n";
	out << "}\n";


	out << "void print_chint(uint64_t v, int i1, int i2) {\n";
	out << "	bitset<64> x(v);\n";	
	out << "	for (int i = i1; i < i2; i++) \n";
	out << "if (x[i] == 1)	cout << i - i1; else cout<<\"X\";\n";	
	out << "cout<<\" \";\n";
	out << "}\n";


	out << "void print_status(){\n";
	out << "cout<<\"LS\"<<endl;\n";
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			out << "print_int(" << LS[i*N + j] << ", " << LS_shift[i*N + j] << ", " << LS_shift[i*N + j] + N << ");\n";
		}
		out << "cout<<endl;\n";
	}
	out << "}\n";

	out << "void print_status_CR(){\n";
	out << "cout<<\"CR\"<<endl;\n";
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			out << "print_chint(" << CR[i*N + j] << ", " << CR_shift[i*N + j] << ", " << CR_shift[i*N + j] + N << ");\n";
		}
		out << "cout<<endl;\n";
	}
	out << "}\n";


	out << "void print_status_RCD(){\n";
	out << "cout<<\"Rows\"<<endl;\n";
	for (auto i = 0; i < N; i++) {
		out << "print_chint(" << Rows[i] << ", " << Rows_shift[i] << ", " << Rows_shift[i] + N << ");\n";
		out << "cout<<endl;\n";
	}
	out << "cout<<endl;\n";
	out << "cout<<\"Columns\"<<endl;\n";
	for (auto i = 0; i < N; i++) {
		out << "print_chint(" << Cols[i] << ", " << Cols_shift[i] << ", " << Cols_shift[i] + N << ");\n";
		out << "cout<<endl;\n";
	}
	out << "cout<<endl;\n";

	out << "cout<<\"MD\"<<endl;\n";	
	out << "print_chint(" << MD_s << ", " << MD_shift << ", " << MD_shift + N << ");\n";
	out << "cout<<endl;\n";
	out << "cout<<\"AD\"<<endl;\n";
	out << "print_chint(" << AD_s << ", " << AD_shift << ", " << AD_shift + N << ");\n";
	out << "cout<<endl;\n";

	out << "}\n";

	out << "void Generate();\n\n";

	out << "void main()\n\{\n";


	out << "\t double t1 = cpuTime();\n";

	out << "\tReset();\n";

	out << "\tGenerate();\n\n";
	out << "\t double t2 = cpuTime();\n";


	out << "cout << \"Total squares count: \" << SquaresCnt << endl;\n";
	out << "cout << \"Time spent: \" << t2 - t1 << endl;\n";

	out << "cout << \"OK\";\n";

	out << "getchar();\n}\n";


	out << "void Generate()\n{\n";

	out << "\tStartProf();\n";

	//vector<int> order9{ 40, 10, 16, 64, 70, 20, 24, 56, 60, 30, 32, 48, 72, 50, 80, 11, 12, 14, 15, 9, 13, 17, 21, 23, 18, 19, 22, 25, 26, 57, 59, 54, 55, 58, 61, 62, 66, 68, 63, 67, 71, 65, 69, 75, 39, 77, 41, 76, 73, 74, 78, 79, 31, 49, 27, 28, 29, 33, 34, 35, 36, 45, 37, 46, 38, 47, 42, 51, 43, 44, 52, 53 };
	//vector<int> order{ 44, 45, 54, 55, 63, 66, 72, 77, 81, 88, 90, 99, 64, 65, 60, 61, 62 , 67, 68, 69, 74, 75, 70, 71, 78, 79, 73, 76, 84, 85, 80, 89, 82, 83, 86, 87, 94, 95, 91, 92, 93, 96, 97, 98, 40, 41, 42, 43, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59 };
	//vector<int> order {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

	gen_inner_cycle_BT_EXT(N, d_size, order, hasN, LS, CR, LS_shift, CR_shift, Rows, Rows_shift, Cols, Cols_shift, MD_s, AD_s, MD_shift, AD_shift, out);
	out << "}\n";

	out.close();
}


void gen_code_find_bt_rc0_diag(int dimension, string filename) {
	ofstream out;
	out.open(filename);
	out << "vector<vector<int>> find_tv_bt(vector<vector<int>> SQ) {\n";
	out << "\t uint64_t AllN = (1 << " << dimension << ")-1;\n";
	out << "\t uint64_t ROW = 0;\n";
	out << "\t uint64_t COL = 0;\n";
	out << "\t uint64_t VAL = 0;\n";
	out << endl;
	out << "\t uint64_t Z1= 0;\n";
	out << "\t uint64_t Z2 = 0;\n";
	out << "\t uint64_t tvcount = 0;\n";
	out << "\t uint64_t r_ind["<<dimension<<"] = 0;\n";
	out << "\t uint64_t c_ind[" << dimension << "] = 0;\n";
	out << "\t uint64_t CL[" << dimension << "] = 0;\n";
	out << "\t uint64_t T[" << dimension << "] = 0;\n";
	out << "\t uint64_t p1[" << dimension << "] = 0;\n";
	out << "\t uint64_t p2[" << dimension << "] = 0;\n";

	out << endl;

	out << "\tuint64_t LS[" << dimension << "][" << dimension << "];\n";
	out << "\t for (int i = 0 ; i <" << dimension << "; i++){\n";
	out << "\t\t for (int j = 0; j <" << dimension << "; j++){\n";
	out << " \t\t\tLS[i][j] = 1ULL << SQ[i][j];\n";
	out << "\t\t\tif (i<5) {\n";
	out << "\t\t\t\t p1[SQ[i][j]] |= 1ULL << (" << dimension << " * i + j);\n";
	out << "\t\t\t}\n";
	out << "\t\t\telse{\n";
	out << "\t\t\t\t p2[SQ[i][j]] |= 1ULL << (" << dimension << " * (i-5) + j);\n";
	out << "\t\t\t}\n";
	out << "\t\t}\n";
	out << "\t}\n";
	// lets rewrite it to start with ad!

	out << "\tfor (int ad_ind = 0; ad_ind <" << dimension << "; ad_ind++){\n";
	out << " \t\t int adc_ind=" << dimension << "- ad - 1;\n";
	out << "\t\tr_ind[0] = 1 << ad;\n";
	out << "\t\tc_ind[0] = 1 << adc_ind;\n";
	out << "\t\tROW |= r_ind[0];\n";
	out << "\t\tCOL |= c_ind[0];\n";
	out << "\t\tint LS_ad = _bit_scan_forward(LS[ad_ind][adc_ind]);\n";
	out << "\t\tZ1 |= p1[LS_ad];\n";
	out << "\t\tZ2 |= p2[LS_ad];\n";
	
	//innercycle_start
	//_blsi_u64 
	out << "\t\tr_ind[1] = ";
	//i stopped somewhere here


	
	//innercycle_end	

	out << "\t\tZ1 ^= p1[LS_ad];\n";
	out << "\t\tZ2 ^= p2[LS_ad];\n";
	out << "\t\tROW ^= r_ind[0];\n";
	out << "\t\tCOL ^= c_ind[0];\n";






	// end




	out << "\tfor (int64_t r_ind[0]=1ULL<<0; r_ind[0] < 1ULL << " << dimension << "; r_ind[0] = r_ind[0]<<1;){\n";	
	out << "\t\tc_ind[0] = r_ind[0];\n";
	out << "\t\ROW |= r_ind[0];\n";
	out << "\t\COL |= r_ind[0];\n";
	out << "\t\t int md_ind = _bit_scan_forward(r_ind[0]);\n";
	out << "\t\tVAL |= LS[md_ind][md_ind];\n";
	out << "\t\tZ1 |= p1[_bit_scan_forward(LS[md_ind][md_ind])];\n";
	out << "\t\tZ2 |= p2[_bit_scan_forward(LS[md_ind][md_ind])];\n";

	// inner cycle


	// inner cycle end

	out << "\t\tZ1 ^= p1[_bit_scan_forward(LS[md_ind][md_ind])];\n";
	out << "\t\tZ2 ^= p2[_bit_scan_forward(LS[md_ind][md_ind])];\n";

	out << "\t\ROW ^= r_ind[0];\n";
	out << "\t\COL ^= r_ind[0];\n";
	out << "\t\tVAL ^= LS[md_ind][md_ind];\n";
	out << "\t}\n";

	out.close();
}






int main() {

//	gen_code_Eduard_BT9_rc0();
	//gen_code_Eduard_rc0();
	//gen_code_Eduard_BT_rc0();
	//gen_code10BT_rc3();

	vector<int> LSmask9{
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1
	};

	vector<int> LSmask8{
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1
	};

	vector<int> LSmask10{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1
	};

	vector<int> LSmask10_3R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	vector<int> LSmask10_4R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	vector<int> LSmask10_5R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	vector<int> LSmask10_6R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};
	vector<int> LSmask10_7R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};
	vector<int> LSmask10_8R{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	vector<int> mask9d{
		1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0
	};
	vector<int> mask9n{
		1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0
	};
	vector<int> mask8d{
		1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0		
	};
	vector<int> mask8_md{
		1,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,1
	};
	vector<int> mask8n{
		1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0		
	};
	vector<int> mask9_md{
		1,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,
		0,0,0,0,0,1,0,0,0,
		0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,0,1
	};

	vector<int> mask10d{
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};
	vector<int> mask10_md{
		1,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,1,0,0,0,0,
		0,0,0,0,0,0,1,0,0,0,
		0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,0,0,1
	};
	vector<int> mask10d_32{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};
	vector<int> mask10d_20{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};
	vector<int> mask10n{
		1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0
	};
	vector<int> mask11n{
		1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,
	};

	vector<int> mask11d{
		1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,
	};

	vector<int> order9{
		40, 10, 16, 64, 70, 20, 24, 56, 60,
		30, 32, 48, 72, 50, 80, 11, 12, 14,
		15, 9, 13, 17, 21, 23, 18, 19, 22,
		25, 26, 57, 59, 54, 55, 58, 61, 62,
		66, 68, 63, 67, 71, 65, 69, 75, 39,
		77, 41, 76, 73, 74, 78, 79, 31, 49,
		27, 28, 29, 33, 34, 35, 36, 45, 37,
		46, 38, 47, 42, 51, 43, 44, 52, 53 };

	vector<int> order10{
		
		11, 18, 22, 27,
		10, 12, 13, 14, 15, 16, 17, 19,
		20, 21, 23, 24, 25, 26, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
		50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
		70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
		80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
		90, 91, 92, 93, 94, 95, 96, 97, 98, 99,		
		};

	vector <vector<int>> sections9{
		{},{},{},{},{},{},{},{},{},//8
		{},{},{},{},{},{},{},{},{},//17
		{},{},{},{},{},{},{},{},{},//26
		{},{},{},{},{},{},{},{},{},//35
		{},{},{},{},{},{},{},{},{},//44
		{},{},{},{},{},{},{ 34,43,52 },{ 27,28,29,33,34,35,49 },{ 45,46,47,51,52,53 },//53
		{ 28,29,33,34,35,36,45 },{ 29,33,34,35,37,46 },{ 33,34,35,38,47 },{ 34,35,42,51 },{ 35,43,52 },{ 44,53 },{},{},{},//62
		{},{},{},{},{},{},{},{},{} //71
	};
	vector <vector<int>> sections9_thr9{
		{},{},{},{},{},{},{},{},{},//8
		{},{},{},{},{},{},{},{},{},//17
		{},{},{},{},{},{},{},{},{},//26
		{},{},{},{},{},{},{},{},{},//35
		{},{},{},{},{},{},{},{},{},//44
		{},{},{},{},{},{},{},{ 49 },{},//53
		{ 28,29,33,34,35,36,45 },{ 29,33,34,35,37,46 },{ 33,34,35,38,47 },{ 34,35,42,51 },{ 35,43,52 },{ 44,53 },{},{},{},//62
		{},{},{},{},{},{},{},{},{} //71
	};
	vector <vector<int>> sections9_thr10{
		{},{},{},{},{},{},{},{},{},//8
		{},{},{},{},{},{},{},{},{},//17
		{},{},{},{},{},{},{},{},{},//26
		{},{},{},{},{},{},{},{},{},//35
		{},{},{},{},{},{},{},{},{},//44
		{},{},{},{},{},{},{},{},{},//53
		{},{ 29,33,34,35 },{ 33,34,35 },{ 35 },{},{},{},{},{},//62
		{},{},{},{},{},{},{},{},{} //71
	};
	analyze_rc2(8, 1, true, false, mask8d, "C:\\Work\\LS8_std.cpp");
	analyze_rc2(8, 1, true, false,  mask8_md, "C:\\Work\\LS8_md.cpp");
	getchar();

	//analyze_sym_rc2(8, 1, true, mask8d, LSmask8, "D:\\LSTests\\testsym_mask8.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_3R, "D:\\LSTests\\testsym_mask10_3r.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_4R, "D:\\LSTests\\testsym_mask10_4r.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_5R, "D:\\LSTests\\testsym_mask10_5r.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_6R, "D:\\LSTests\\testsym_mask10_6r.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_7R, "D:\\LSTests\\testsym_mask10_7r.cpp");
	//analyze_sym_rc2(10, 1, true, mask10d, LSmask10_8R, "D:\\LSTests\\testsym_mask10_8r.cpp");
//	analyze9(9, 1);
	/*vector<int> order_exp;
	vector<vector<int>> sections;
	vector<int> has9;
	analyze(9, 1, 0, true, mask9d, order_exp, 50, 80, 10, 20, 2, sections, has9);
	gen_codeBT_rc7(9, true, order_exp, has9, sections9, "D:\\LStests\\gen9bounds_exp_ED.cpp");

	analyze_rc2(9, 1, true, mask9d, "D:\\LSTests\\gen9d_bt2new.cpp");

	analyze_rc1(9, 1, true, mask9d, "D:\\LSTests\\gen9d_new.cpp");
	//analyze_rc1(10, 1, true, mask10d, "D:\\LSTests\\gen10d_new.cpp");
	
//	gen_codeBT_rc7(9, true, order_exp, has9, sections, "D:\\LStests\\gen9bounds_exp_9_10_40_65_2_D.cpp");
	vector <vector<int>> sections10;

	vector<int> ord10_32;
	analyze(10, 1, 0, false, mask10n, ord10_32, 75, 90, 10, 20, 2, sections10, has9);

	gen_codeBT_rc7(10, false, ord10_32, has9, sections10, "D:\\LSTests\\gen10_diag.cpp");*/
	//gen_codeBT_rc6(10, ord10_32, "D:\\LSTests\\gen10_32.cpp");
	//gen_codeBT_EXT(9, 10, true, order9, has9, emptysect9, "D:\\LStests\\9ext.cpp");




	//gen_codeBT_rc7(9, order9, has9, sections, "D:\\LStests\\gen9bounds_exp_9_40_65_2c.cpp");
	//analyze(10, 1, 0, true, mask10d, order_exp, 45, 70, 10, 2, sections, has9);
//	vector <vector<int>> sections10_empty(sections.size());
	//gen_codeBT_rc7(10, true, order_exp, has9, sections, "D:\\LStests\\gen10_diag_10_45_70_2.cpp");
	gen_codeBT_rc6(10, order10, "D:\\LSTests\\gen10.cpp");
	//gen_codeBT_rc7(10, true, order_exp, has9, sections10_empty, "D:\\LStests\\gen10_diag_e.cpp");

	

	vector<int> order10_dummy(90);
	for (int i = 10; i < 100; i++) {
		order10_dummy[i - 10] = i;
	}

	//gen_code9BT_rc5(10, order10_dummy, "D:\\LSTests\\Gen10dummy.cpp");


	//	gen_code9BT_rc5(9, order9, "C:\\work\\tests\\gen9rc5.cpp");
	//gen_codeBT_rc6(9, order9, "D:\\LSTests\\Gen9rc6.cpp");


	/*gen_code_Eduard_rc0();
	gen_code_Eduard_BT_rc0();
	gen_code_Eduard_BT_rc1();
	gen_code_Eduard_BT_rc2();
	gen_code_Eduard_BT9_rc0();
	gen_code9BT_rc2();
	gen_code10BT_rc3();
	gen_code9BT_rc3();
	gen_code9BT_rc4();*/
	cout << "OK";
	getchar();
}


