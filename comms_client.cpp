#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "UDPcomms.h"
#include "config.h"
#include <fstream>

using namespace std;
using namespace cv;

int main(void)
{	
	UDPcomms comm;
	comm.client();
	//Read from object.txt
	system("scp pratik@192.168.0.20:~/AutoBot18/object.txt ./");
	fstream file;
	int array[15];
	int x;
	file.open("object.txt");
        int i = 0;
	while (file>>x) {
		cout << x << endl;
		//for (int i = 0; i < 4; i++) {
			array[i] = x;
			i++;
		//}
		}
	for (int i = 0; i < 15; i++) {
		cout << array[i] <<' ';
	}
	
	

	file.close();
	//system("pause");
	return 0;
}
