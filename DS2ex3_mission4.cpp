// 資訊二甲 第四組 10827138 廖翊中  10827139 黃云潔

#include <iostream>
#include <fstream>               // ofstream, ifstream管理檔案的輸出輸入 
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <vector>  
#include <math.h>
#include <ctime>

using namespace std ;

typedef struct List{
	char sid[10] ;
	char sname[10] ;
	unsigned char score[6] ;
	float average ;
} listType ;

typedef struct Student{
	int location ;
	int hvalue ;	// 雜湊值
	string sid ; 
	string sname ;
	float average ;
} studentType ;

typedef struct StdID{
	string sID ;
	string sname ;
	long long sPw ;
	struct StdID *next ;
} stdIDType ;

typedef struct StdPw{
	int location ;
	int hvalue ;	// 雜湊值
	stdIDType *head ;
} stdPwType ;


class ReadFile {
	
	public : 
	
		int readInt() {                          // 讀入正確指令 
			char c = '\0' ;
			int n = 0 , re = 0;                  // re 回傳數值 
			
			scanf( "%c", &c );	
			while( c == '\n' || c == ' ' || c == '\t' ) {
				scanf( "%c", &c );	
			} // while
			
			while ( c >= '0' && c <= '9' ) {
				n = c - '0';
				re = re * 10 + n;
			    scanf( "%c", &c );	
			} //while
			
			if ( c != '\n' && c != ' ' && c != '\t' )
			    re = -1;
			
			return re ;
			
		} // readInt
	
		int StringToInt( string numStr ) {       // string to int
		
			int tempNum = 0 ;
			for ( int i = 0 ; i < numStr.size() ; ++i ) {
				
				if ( numStr[i] >= '0' && numStr[i] <= '9' ) {
					tempNum = ( numStr[i] - '0') + ( tempNum * 10 ) ;
				} // if
				
			} // for
			
			return tempNum;
			
		} //stringToInt
		
		int NextPrime( float n ) {				 // 找到大於 n 的最小質數 
		    int i = (int)n ; 
		    
		    i++;
		    while ( i >= n ) {
		        for ( int j = 2 ; j <= i ; j++ ) {
		            if( i == j )
		                return i;
		            if( i % j == 0 )
		                break;
		        } // for
		        
		    	i++;
		    } // while 
		    
		} // FindNextPrime
		
		bool ReadTheFile( string &inputStr ) {	 // 讀入bin檔，若只有txt就強制做一個bin檔 
			string fileName ;
			string nameTxt ;
			char* finName ;
			char* finNameTxt ;                         		    // 傳統字串 
			if ( strcmp( inputStr.c_str(), "0" ) == 0 )
				return false ;
			
			fileName = "input" + inputStr + ".bin" ;
			finName = new char[ fileName.size()+1 ] ;
			strcpy( finName, fileName.c_str() ) ;              // 檔名的變數型態只能是傳統字串 char[], char* 			
			ifstream fin( finName ) ;                       // file in
			
			if ( !fin ) {									// 沒有binary檔 
				nameTxt = "input" + inputStr + ".txt" ;
				finNameTxt = new char[ nameTxt.size()+1 ] ;
				strcpy( finNameTxt, nameTxt.c_str() ) ;              // 檔名的變數型態只能是傳統字串 char[], char* 			
				ifstream finTxt( finNameTxt ) ;                       // file in
	
				if ( !finTxt ) {
					cout << "\n### input" << inputStr << " does not exist! ###\n" ;
					return false ;
				} // if
				else {												// 只有txt檔案   	老師的 Text2Binary() 
		
					fstream outFile ;
					outFile.open( fileName.c_str(), fstream::out | fstream::binary ) ;
					if ( outFile.is_open() ) {
						char rBuf[255] ; 							// 存放一筆完整的資料 
						
						while ( finTxt.getline( rBuf, 255, '\n' ) ) {
							
							string temp ;
							listType oneStudent ;
							int cNo = 0, pre = 0, pos = 0 ;			// cNo是case的選擇，pre一開始是頭，pos是尾 
							temp.assign(rBuf) ;						// 複製一份資料 
							pos = temp.find_first_of('\t', pre) ;
							while ( pos != string::npos ) {
								
								switch( ++cNo ) {
									case 1 :
										strcpy( oneStudent.sid, temp.substr(pre, pos-pre).c_str() ) ;
										break ;
										
									case 2 :
										strcpy( oneStudent.sname, temp.substr(pre, pos-pre).c_str() ) ;
										break ;
										
									case 3 :
										oneStudent.score[cNo-3] = atoi(temp.substr(pre, pos-pre).c_str() ) ;
										break ;
										
								} // end switch
								
								pre = ++pos ;
								pos = temp.find_first_of('\t', pre) ;
								
							} // while
							
							pos = temp.find_last_of('\t') ;
							oneStudent.average = atof(temp.substr(pos+1).c_str() ) ;
							outFile.write( (char*)&oneStudent, sizeof(oneStudent) ) ;
							
						} // while
						
						outFile.close() ;
					} // if
					
					finTxt.close() ;
				} // else
			} // if
			
			return true ;
		} // Read The File
		
		float CountComparisons( vector<studentType> inform ) {	// 計算不存在值的平均比較次數 
			int sum = 0 ;		 // sum 計算不存在
			
			for( int i = 0 ; i < inform.size() ; i++ ) {
				
				int j = i ;
				int count = 0 ;
				while( inform[j].hvalue != 0 ) {		// 雜湊表該key中有東西 
					count++ ;
					j++ ;
					
					if( j == inform.size() )
						j = 0 ;
						
				} // while
				
				sum = sum + count ;
				
			} // for
			
			return (float) sum / inform.size() ;
			
		} // CountComparisons
		
		float CountQComparisons( vector<studentType> inform ) {	// 計算不存在值的平均比較次數 
			int sum = 0 ;		 // sum 計算不存在
			
			for( int i = 0 ; i < inform.size() ; i++ ) {
				
				int j = i ;
				int t = 0 ;
				int count = 0 ;
				while( inform[j].hvalue != 0 ) {		// 雜湊表該key中有東西 
					t++ ;
					count++ ;
					j = i ;
					j = j + pow( t, 2 );
					
					if( j >= inform.size() )
						j = j % inform.size() ;
						
					if( count > inform.size()*100 )
						break ;
				} // while
				
				if ( count > inform.size()*100 ) ;
				else{
					sum = sum + count ;
				} // else
				
			} // for
			
			return (float) sum / inform.size() ;
			
		} // CountQComparisons
		
}; // ReadFile

class LinearProbe{
	
	public :
		
		vector<studentType> linear ;
		
		void LinaerFileOut( vector<studentType> inform, string inputStr ) {
			
			ofstream lout( ( "linear" + inputStr + ".txt" ).c_str() ) ;
			lout << " -- Hash Table X --- (linear probing)" << endl ;
			
			for( int i = 0 ; i < inform.size() ; i++ ) {
				
				lout << "[  " << setw(3) << inform[i].location << "]" ;
				if( inform[i].hvalue != NULL )
					lout << setw(11) << inform[i].hvalue << "," << setw(11) << inform[i].sid << "," << setw(11) << inform[i].sname << "," << setw(11) << inform[i].average ;
				lout << endl ;
				
			} // for
			
			lout.close() ;
			
		} // LinaerFileOut
		
		bool LinearProbing() {
			
			ReadFile temp ;
			string inputStr ;
			int hashTableSize = 0 ;
			int countSuccessful = 0 ;
			float u, s ;
			double start = 0, end = 0, sumOfTime = 0 ;
			
			cin >> inputStr ;
			if ( temp.ReadTheFile( inputStr ) ) {	 
				fstream binFile ;
				listType oneStudent ;
				studentType tempStudent ;
				int studentNo = 0 ;			// 計算學生數
				int tempKey = 0 ;			// 計算每個學生的key值 
				long long sumOfStudent ;	// 學號字母相乘總和 
				int tempCount = 0 ;
				
				binFile.open( ( "input" + inputStr + ".bin" ).c_str(), fstream::in | fstream::binary ) ;
				if ( binFile.is_open() ) {
					binFile.seekg( 0, binFile.end ) ;
					studentNo = binFile.tellg() / sizeof(oneStudent) ;
					
					hashTableSize = temp.NextPrime( studentNo*1.2 ) ;			// 大於1.2倍資料筆數的最小質數 
					
					linear.resize( hashTableSize ) ;							// 給定雜湊表大小 
					for( int i = 0 ; i < hashTableSize ; i++ ) {
						 linear[i].location = i ;
					} // for
					
					binFile.seekg( 0, binFile.beg ) ;
					for( int i = 0 ; i < studentNo ; i++ ) {
						
						start = clock() ;
						binFile.read( (char*)&oneStudent, sizeof(oneStudent) ) ;
						sumOfStudent = (long)oneStudent.sid[0] ;
						
						for( int j = 1 ; (long)oneStudent.sid[j] != 0 ; j++ ) {			// 每個學號字母相乘 
							sumOfStudent = sumOfStudent * (long)oneStudent.sid[j] ;
						} // for
						
						tempKey = sumOfStudent % hashTableSize ;
						
						int k = tempKey ;
						while( linear[k].hvalue != NULL ) {		// 雜湊表該key中有東西 
							countSuccessful ++ ;
							k++ ;
							tempCount++ ;
							
							if( k == hashTableSize )
								k = 0 ;
						} // while
						
						// 跳出後 k 為找到空位 
						
						linear[k].hvalue = tempKey ;
						linear[k].sid = oneStudent.sid ;
						linear[k].sname = oneStudent.sname ;
						linear[k].average = oneStudent.average ;
						countSuccessful ++ ;
						
						end = clock() ;
						sumOfTime = sumOfTime + (end-start) ;
					} // for
					
					cout << setiosflags(ios::fixed) << setprecision(50) << (sumOfTime)/studentNo << endl ;
					
					LinaerFileOut( linear, inputStr ) ;		// 輸出雜湊表 
					cout << "Hash Table X has been created.\n" ;					
					cout << "unsuccessful search: " << setiosflags(ios::fixed) << setprecision(4) << temp.CountComparisons( linear ) << " comparisons on average" << endl ;	// 印出小數點後4位 
					cout << "successful search: " << setiosflags(ios::fixed) << setprecision(4) << (float)countSuccessful/studentNo << " comparisons on average" << endl ;		// 印出小數點後4位
				
				} // if
				
				linear.clear() ;
				binFile.close() ;
			} // if
			else 
				cout << "\n### input" << inputStr << " does not exist! ###\n" ;
		
		} // LinearProbing
	
}; // LinearProbe 

class DoubleHash {
	
	public :
		
		vector<studentType> doubleHash ;
		
		void DoubleHashFileOut( vector<studentType> inform, string inputStr ) {
			
			ofstream dout( ( "double" + inputStr + ".txt" ).c_str() ) ;
			dout << " -- Hash Table Y --- (double probing)" << endl ;
			
			for( int i = 0 ; i < inform.size() ; i++ ) {
				
				dout << "[  " << setw(3) << inform[i].location << "]" ;
				if( inform[i].hvalue != NULL )
					dout << setw(11) << inform[i].hvalue << "," << setw(11) << inform[i].sid << "," << setw(11) << inform[i].sname << "," << setw(11) << inform[i].average ;
				dout << endl ;
			} // for
			
			dout.close() ;
			
		} // DoubleHashFileOut
		
		void DoubleHashing() {
			
			ReadFile temp ;
			string inputStr ;
			int hashTableSize = 0 ;
			float u, s ;
			
			cin >> inputStr ;
			if ( temp.ReadTheFile( inputStr ) ) {	 
				fstream binFile ;
				listType oneStudent ;
				studentType tempStudent ;
				int studentNo = 0 ;			// 計算學生數
				int tempKey = 0 ;			// 計算每個學生的key值 
				int stepKey = 0 ;
				int highestStep = 0 ;
				int countSuccessful = 0 ;
				long long sumOfStudent ;	// 學號字母相乘總和 
				
				binFile.open( ( "input" + inputStr + ".bin" ).c_str(), fstream::in | fstream::binary ) ;
				if ( binFile.is_open() ) {
					binFile.seekg( 0, binFile.end ) ;
					studentNo = binFile.tellg() / sizeof(oneStudent) ;		// 資料總筆數
					
					hashTableSize = temp.NextPrime( studentNo*1.2 ) ;			// 大於1.2倍資料總筆數的最小質數 
					highestStep = temp.NextPrime( studentNo/3 ) ;
					doubleHash.resize( hashTableSize ) ;							// 給定雜湊表大小 
					for( int i = 0 ; i < hashTableSize ; i++ ) {
						 doubleHash[i].location = i ;
					} // for
					
					binFile.seekg( 0, binFile.beg ) ;
					
					for( int i = 0 ; i < studentNo ; i++ ) {
						
						binFile.read( (char*)&oneStudent, sizeof(oneStudent) ) ;
						sumOfStudent = (long)oneStudent.sid[0] ;
						
						for( int j = 1 ; (long)oneStudent.sid[j] != 0 ; j++ ) {			// 每個學號字母相乘 
							sumOfStudent = sumOfStudent * (long)oneStudent.sid[j] ;
						} // for
						
						
						tempKey = sumOfStudent % hashTableSize ;
						stepKey = highestStep - ( sumOfStudent % highestStep ) ;
						
						int k = tempKey ;
						
						while( doubleHash[k].hvalue != 0 ) {		// 雜湊表該key中有東西
							countSuccessful ++ ; 
							k = k + stepKey ;
							
							if ( k >= hashTableSize )
								k = k % hashTableSize ;
							
						} // while
						
						// 跳出後 k 為找到空位 
						
						doubleHash[k].hvalue = tempKey ;
						doubleHash[k].sid = oneStudent.sid ;
						doubleHash[k].sname = oneStudent.sname ;
						doubleHash[k].average = oneStudent.average ;
						countSuccessful ++ ;
					} // for
						
					DoubleHashFileOut( doubleHash, inputStr ) ;		// 輸出雜湊表 
					cout << "Hash Table Y has been created.\n" ;					
					cout << "successful search: " << setiosflags(ios::fixed) << setprecision(4) << (float)countSuccessful/studentNo << " comparisons on average" << endl ;		// 印出小數點後4位
					
				} // if
				
				doubleHash.clear() ;
				binFile.close() ;
			} // if
			else 
				cout << "\n### input" << inputStr << " does not exist! ###\n" ;
				
		} // Double Hashing
		
}; // DoubleHash

class QuadraticProbe {
	
	public :
		
		vector<studentType> quadratic ;
		
		void QuadraticFileOut( vector<studentType> inform, string inputStr ) {
			
			ofstream qout( ( "quadratic" + inputStr + ".txt" ).c_str() ) ;
			qout << " --- Hash Table Z --- (quadratic probing)" << endl ;
			
			for( int i = 0 ; i < inform.size() ; i++ ) {
				
				qout << "[  " << setw(3) << inform[i].location << "]" ;
				if( inform[i].hvalue != 0 )
					qout << setw(11) << inform[i].hvalue << "," << setw(11) << inform[i].sid << "," << setw(11) << inform[i].sname << "," << setw(11) << inform[i].average ;
				qout << endl ;
				
			} // for
			
			qout.close() ;
			
		} // QuadraticFileOut
		
		bool QuadraticProbing() {
			
			ReadFile temp ;
			string inputStr ;
			int hashTableSize = 0 ;
			int countSuccessful = 0, tempCount = 0 ;
			bool isFailed = false ;
			float u, s ;
			double start = 0, end = 0, sumOfTime = 0 ;
			
			cin >> inputStr ;
			if ( temp.ReadTheFile( inputStr ) ) {	 
				fstream binFile ;
				listType oneStudent ;
				studentType tempStudent ;
				int studentNo = 0 ;			// 計算學生數
				int tempKey = 0 ;			// 計算每個學生的key值 
				long long sumOfStudent ;	// 學號字母相乘總和 
				
				binFile.open( ( "input" + inputStr + ".bin" ).c_str(), fstream::in | fstream::binary ) ;
				if ( binFile.is_open() ) {
					binFile.seekg( 0, binFile.end ) ;
					studentNo = binFile.tellg() / sizeof(oneStudent) ;
					
					hashTableSize = temp.NextPrime( studentNo*1.2 ) ;			// 大於1.2倍資料筆數的最小質數 
					
					quadratic.resize( hashTableSize ) ;							// 給定雜湊表大小 
					for( int i = 0 ; i < hashTableSize ; i++ ) {
						 quadratic[i].location = i ;
					} // for
					
					binFile.seekg( 0, binFile.beg ) ;
					
					for( int i = 0 ; i < studentNo ; i++ ) {
						
						start = clock() ;
						binFile.read( (char*)&oneStudent, sizeof(oneStudent) ) ;
						sumOfStudent = (long)oneStudent.sid[0] ;
						
						for( int j = 1 ; (long)oneStudent.sid[j] != 0 ; j++ ) {			// 每個學號字母相乘 
							sumOfStudent = sumOfStudent * (long)oneStudent.sid[j] ;
						} // for
						
						tempKey = sumOfStudent % hashTableSize ;
						
						int k = tempKey ;
						int t = 0 ;
						tempCount = 0 ;
						while( quadratic[k].hvalue != 0 ) {		// 雜湊表該key中有東西 
							t++ ;
							tempCount ++ ;
							//k = tempKey + pow( t, 2 );
							k = tempKey + ( t * t ) ;
							
							if( k >= hashTableSize )
								k = k % hashTableSize ;
							
							//if( tempCount > hashTableSize*100 )
							//	break ;
						} // while
						
						// 跳出後 k 為找到空位 
						
						if ( tempCount > hashTableSize*100 ) {
							cout << "### Failed at [" << i << "]. ###\n" ;
							isFailed = true ;
						} // 
						else{
							quadratic[k].hvalue = tempKey ;
							quadratic[k].sid = oneStudent.sid ;
							quadratic[k].sname = oneStudent.sname ;
							quadratic[k].average = oneStudent.average ;
							countSuccessful = countSuccessful + tempCount + 1 ;
						} // else
						
						end = clock() ;
						sumOfTime = sumOfTime + (end-start) ;
					} // for
					
					cout << setiosflags(ios::fixed) << setprecision(50) << (sumOfTime)/studentNo << endl ;
						
					QuadraticFileOut( quadratic, inputStr ) ;		// 輸出雜湊表 
					cout << "Hash Table Z has been created.\n" ;
					if( !isFailed )	{			
						cout << "unsuccessful search: " << setiosflags(ios::fixed) << setprecision(4) << temp.CountQComparisons( quadratic ) << " comparisons on average" << endl ;	// 印出小數點後4位 
						cout << "successful search: " << setiosflags(ios::fixed) << setprecision(4) << (float)countSuccessful/studentNo << " comparisons on average" << endl ;		// 印出小數點後4位
					}
				} // if
				
				quadratic.clear() ;
				binFile.close() ;
			} // if
			else 
				cout << "\n### input" << inputStr << " does not exist! ###\n" ;
		
		} // QuadraticProbing
	
}; // QuadraticProbe 

class PassWordSet {
	
	public:
		
		vector<stdPwType> pwlist ;
	
		bool findAccNum( string inputStdNum ) {
			
			long long sumOfID ; 
			stdIDType *walk ;
			int tempKey ;
			
			sumOfID = inputStdNum[0] ;
			
			for ( int i = 1 ; i < inputStdNum.size() ; i++ ) 
				sumOfID = sumOfID * inputStdNum[i] ; 
			
			tempKey = sumOfID % ( pwlist.size() ) ;
					
			int k = tempKey ;
			walk = pwlist[k].head ;
			while( walk != NULL ) {
				if ( inputStdNum == walk->sID )
					break ;
				walk = walk->next ;
			} // while
			
			if ( walk != NULL ) {
				cout << "This account has been registered.\n" ;
				return false ;
			} // if
			else 
				return true ;
			
		} // findAccNum
	
		bool isQualified( string inputPw ) {
			if ( inputPw.size() >= 5 && inputPw.size() <=8 )
				return true ;
			else 
				return false ;
		} // isQualified
	
		void createHashList() {
			
			ReadFile temp ;
			string inputStr = "310" ;
			int hashTableSize = 0 ;
			int fileCount = 0 ;
		
			if ( temp.ReadTheFile( inputStr ) ) {
				fstream binFile ;
				listType oneStudent ;
				studentType tempStudent ;
				int studentNo = 0 ;			// 計算學生數
				int tempKey = 0 ;			// 計算每個學生的key值 
				long long sumOfStudent ;			// 密碼字母相乘總和 
				long long encPw ;
				int sumOfPw ;
				int tempR ;
				char* tempS ;
				string tempC ;
				string pwstr ;
				stdIDType *walk ;
				
				binFile.open( ( "input" + inputStr + ".bin" ).c_str(), fstream::in | fstream::binary ) ;
				
				if ( binFile.is_open() ) {
					binFile.seekg( 0, binFile.end ) ;
					studentNo = binFile.tellg() / sizeof(oneStudent) ;
					hashTableSize = temp.NextPrime( studentNo*1.5 ) ;			// 大於1.5倍資料筆數的最小質數 
					pwlist.resize( hashTableSize ) ;							// 給定雜湊表大小 
					for( int i = 0 ; i < hashTableSize ; i++ ) {
						 pwlist[i].location = i ;
					} // for				
					binFile.seekg( 0, binFile.beg ) ;
					
					for( int i = 0 ; i < studentNo ; i++ ) {
						pwstr = "\0" ;
						binFile.read( (char*)&oneStudent, sizeof(oneStudent) ) ;
						sumOfStudent = (long)oneStudent.sid[0] ;
						
						for( int j = 1 ; (long)oneStudent.sid[j] != 0 ; j++ ) {			// 每個學號字母相乘 
							sumOfStudent = sumOfStudent * (long)oneStudent.sid[j] ;
						} // for
						
						tempKey = sumOfStudent % hashTableSize ;
						
						for ( int j = 0 ; j < 8 ; j++ ) {						// 給定亂數十位數密碼 
							tempR = rand()%10 ;
							itoa( tempR, tempS, 10 ) ;
							pwstr = pwstr + tempS ;
						} // for
						
						pwstr = pwstr + " " ;				// 密碼+空白字元
						encPw = 0 ;
						for ( int k = 0 ; k < pwstr.size() ; k++ ) {
							encPw = encPw*100 + pwstr[k] ;
						} // for
						encPw = encPw / 1313131 + pwstr.size() ;
						
						int k = tempKey ;
						
						if ( pwlist[k].hvalue != 0 ) {		// 雜湊表該key中有東西 
							walk = pwlist[k].head ;
							while( walk->next != NULL ) {
								walk = walk->next ;
							} // while
							
							walk->next = new stdIDType ;
							walk->next->sID = oneStudent.sid ;
							walk->next->sname = oneStudent.sname ;
							walk->next->sPw = encPw ;
							walk->next->next = NULL ;
						} // if 
						else {
							pwlist[k].hvalue = tempKey ;
							pwlist[k].head = new stdIDType ;
							pwlist[k].head->sID = oneStudent.sid ;
							pwlist[k].head->sname = oneStudent.sname ;
							pwlist[k].head->sPw = encPw ;
							pwlist[k].head->next = NULL ;
						} // else
						
					} // for
					 
				} // if
				
			} // if
			
		} // createHashList
	
		void PassWordSetting () {
	
			ReadFile temp ;
			listType oneStudent ;
			string inputStdNum ;
			string inputPw ;
			string inputName ;
			long long sumOfID ; 
			long long encPw ;
			int sumOfPw ;
			int tempR ;
			char* tempS ;
			string pwstr ;
			stdIDType *walk ;
			int tempKey ;
			int k = 0 ;
			
			cout << "\n【註冊】\n\n請輸入學號： " ;
			cin >> inputStdNum ;
			
			while( !findAccNum( inputStdNum ) ) {
				cout << "\n請輸入學號： " ;
				cin >> inputStdNum ;
			} // while
			
			cout << "\n請輸入密碼(限5~8碼)： " ;
			cin >> inputPw ;
			
			while ( !isQualified( inputPw ) ) {
				cout << "\n請輸入密碼(限5~8碼!!!)： " ;
				cin >> inputPw ;
			} // while
			
			if ( findAccNum( inputStdNum ) && isQualified( inputPw ) ) {
				cout << "\n請輸入姓名： " ;
				cin >> inputName ;
	
				sumOfID = inputStdNum[0] ;
				
				for( int j = 1 ; (long)inputStdNum[j] != 0 ; j++ ) {			// 每個學號字母相乘 
					sumOfID = sumOfID * (long)inputStdNum[j] ;
				} // for
				
				tempKey = sumOfID % pwlist.size() ;
	
				pwstr = inputPw + " " ;				// 密碼+空白字元
				encPw = 0 ;
				for ( int k = 0 ; k < pwstr.size() ; k++ ) {
					encPw = encPw*100 + pwstr[k] ;
				} // for
				encPw = encPw / 1313131 + pwstr.size() ;
				
				k = tempKey ;
				
				if ( pwlist[k].hvalue != 0 ) {		// 雜湊表該key中有東西 
					walk = pwlist[k].head ;
					while( walk->next != NULL ) {
						walk = walk->next ;
					} // while
					
					walk->next = new stdIDType ;
					walk->next->sID = inputStdNum ;
					walk->next->sname = inputName ;
					walk->next->sPw = encPw ;
					walk->next->next = NULL ;
				} // if 
				else {
					pwlist[k].hvalue = tempKey ;
					pwlist[k].head = new stdIDType ;
					pwlist[k].head->sID = inputStdNum ;
					pwlist[k].head->sname = inputName ;
					pwlist[k].head->sPw = encPw ;
					pwlist[k].head->next = NULL ;
				} // else
			
			} // if
			
			cout << "\n存放位址： [  " << setw(3) << pwlist[k].location << "]" ;
			cout << " / 學號：" << setw(11) << pwlist[k].head->sID << " / 姓名：" << setw(11) << pwlist[k].head->sname << " / 加密密碼：" << setw(11) << pwlist[k].head->sPw ;
			cout << endl ;
	
			cout << endl << "< 註冊完成！可嘗試登入 >\n" ;
		} // PassWordSetting
	
		bool LogInTest() {
			
			string inputStdNum ;
			string inputPw ;
			long long sumOfID ; 
			long long encPw ; 
			int tempKey ; 
			string pwstr ;
			stdIDType *walk ;
			
			cout << "\n【登入】\n\n請輸入學號： " ;
			cin >> inputStdNum ;
			
			cout << "\n請輸入密碼： " ;
			cin >> inputPw ;
			
			sumOfID = inputStdNum[0] ;
			
			for ( int i = 1 ; i < inputStdNum.size() ; i++ ) 
				sumOfID = sumOfID * inputStdNum[i] ; 
			
			tempKey = sumOfID % ( pwlist.size() ) ;
			
			pwstr = inputPw + " " ;				// 密碼+空白字元
			encPw = 0 ;
			for ( int k = 0 ; k < pwstr.size() ; k++ ) 
				encPw = encPw*100 + pwstr[k] ;
			encPw = encPw / 1313131 + pwstr.size() ;
			
			if ( pwlist[tempKey].hvalue != 0 ) {		// 雜湊表該key中有東西 
				walk = pwlist[tempKey].head ;
				while( walk != NULL ) {
					if ( inputStdNum == walk->sID )
						break ;
					walk = walk->next ;
				} // while
				
				if ( walk != NULL ) {
					if ( encPw == walk->sPw ) {
						cout << endl << "< 登入success！歐耶！ 歡迎 " << walk->sname << " ！ >" << endl ;
						return true ;
					} // if
					else {
						cout << endl << "< 學號或密碼錯誤！ 請重新登入 >" << endl ;
						return false ; 
					} // else
				} // if
				else {
					cout << endl << "< 學號或密碼錯誤！ 請重新登入 >" << endl ;
					return false ;
				} // else
					
			} // if
			else {
				cout << endl << "< 學號或密碼錯誤！ 請重新登入 >" << endl ;
				return false ;
			} // else
			
		} // LogInTest
	
		void ChooseOp() {
			
			ReadFile temp ;
			int op = 0 ;
			
			cout << "\n【請選擇註冊或登入】 ( 0: quit, 1: register, 2: log in )： " ;
			op = temp.readInt() ;
			
			while ( op != 0 ) {
				
				if ( op == 1 ) {
					PassWordSetting() ;
				} // if
				else if ( op == 2 ) {
					if ( LogInTest() )
						break ;
				} // if
				
				cout << "\n【請選擇註冊或登入】 ( 0: quit, 1: register, 2: log in )： " ;
				op = temp.readInt() ;
				
			} // while
			
			cout << endl<< "< Bye! >\n" ;
			
		} // Choose
	
}; // PassWordSet


int main( int argc, char** argv ) {
	
	int cmd = 0, count = 0 ;
	bool quit = false ;
	bool doneHashList = false ; 
	ReadFile temp ;
	LinearProbe lnp ;
	DoubleHash dbh ;
	QuadraticProbe qdp ;
	PassWordSet pws ;
	
	while( !quit ) {
		cout << "\n**** Hash Table Builder ****\n* 0. QUIT                  *\n* 1. Linear Probing        *\n* 2. Double Hash      	   *\n* 3. Quadratic probing     *\n* 4. Register and Log in   *\n*****************************\nInput a choice(0, 1, 2, 3, 4 ): " ;

		cmd = temp.readInt() ;
		
		if ( cmd == 1 ) {
			cout << "\nInput a file number ([0] Quit): " ;
			lnp.LinearProbing() ;
		} // if
		else if ( cmd == 2 ) {
			cout << "\nInput a file number ([0] Quit): " ;
			dbh.DoubleHashing() ;
		} // else if
		else if ( cmd == 3 ) {
			cout << "\nInput a file number ([0] Quit): " ;
			qdp.QuadraticProbing() ;
		} // else if
		else if ( cmd == 4 ) {
			if ( !doneHashList ) {
				pws.createHashList() ;
				pws.ChooseOp() ;
				doneHashList = true ;
			} // if
			else {
				pws.ChooseOp() ;
			} // else
		} // else if
		else if ( cmd == 0 )
			quit = true ;
		else 
			cout << "\nCommand does not exist!\n" ;
		
		count = 0 ;
		
	} // while
	
	system( "pause" ) ;
	return 0;
	
} // main





