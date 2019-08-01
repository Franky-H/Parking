#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	int floor;
	int num;
} Place;

typedef struct {
	Place parked_place;
	char car_num[4];
} Car;

typedef struct {
	Place reserved_place;
	char car_num[4];
} Reserve;

char parking_lots[3][10][BUFSIZ];
Car parked_cars[30];
Reserve reserved_cars[30];

int main_menu() {
	int menu;
	
	puts("1. 주차");
	puts("2. 출차");
	puts("3. 지정차량 등록");
	puts("4. 주차현황 출력");
	puts("0. 종료");
	printf("> 번호 선택 : ");
	scanf("%d", &menu);

	return menu;
}

void init_parking_lot() {
	int i, j, k;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 10; j++) {
			sprintf(((char *)parking_lots[i][j]), "%3d ", j + 1);
		}
	}
}

void print_parking_lot(int floor) {
	int i, j, k;
	switch(floor){
		case 0:
			for (i = 0; i < 3; i++) {
				printf("[B%d층]========================\n", i + 1);
				for (j = 0; j < 10; j++) {
					printf("[%s]", parking_lots[i][j]);
					if (j == 4)
						printf("\r\n");
				}
				printf("\r\n");
			}
			break;
		case 1:
			printf("[B1층]========================\n");
			for (j = 0; j < 10; j++) {
				printf("[%s]", parking_lots[0][j]);
				if (j == 4)
					printf("\r\n");
			}
			printf("\r\n");
			break;
		case 2:
			printf("[B2층]========================\n");
			for (j = 0; j < 10; j++) {
				printf("[%s]", parking_lots[1][j]);
				if (j == 4)
					printf("\r\n");
			}
			printf("\r\n");			
			break;
		case 3:
			printf("[B3층]========================\n");
			for (j = 0; j < 10; j++) {
				printf("[%s]", parking_lots[2][j]);
				if (j == 4)
					printf("\r\n");
			}
			printf("\r\n");			
			break;
	}
	
}

int available_lots(char floor[10][BUFSIZ]){
	int cnt = 0;
	int i, j, k;
	for(i = 0; i < 10; i++){
		if(strchr(floor[i], ' ')){
//			printf("%d 공백 \n", i);
			cnt++;
		}
	}
	return cnt;	
}

int end_process(int c_cnt, int r_cnt) {
	// 파일에 쓰기
	// 프로그램 종료
	FILE * fp;
	
	int i, j;
	
	puts("프로그램을 종료합니다.");
	
	fp = fopen("parking_lot.txt", "w");
	
	if(fp == NULL)
		return -1;
		
//	puts("주차된 차량 목록");
	fprintf(fp, "주차된 차량 목록\n");
	fprintf(fp, "%d\n", c_cnt);
	for(i = 0; i < c_cnt; i++){
//		printf("%s %d %d\n", parked_cars[i].car_num, parked_cars[i].parked_place.floor, parked_cars[i].parked_place.num);
		fprintf(fp, "%s %d %d\n", parked_cars[i].car_num, parked_cars[i].parked_place.floor, parked_cars[i].parked_place.num);
	}
	
	fprintf(fp, "예약된 차량 목록\n");
	fprintf(fp, "%d\n", r_cnt);
	for(i = 0; i < r_cnt; i++){
		fprintf(fp, "%s %d %d\n", reserved_cars[i].car_num, reserved_cars[i].reserved_place.floor, reserved_cars[i].reserved_place.num);
	}
	
	fprintf(fp, "층별 차량 목록\n");
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 10; j++) {
			fprintf(fp, "%s ", parking_lots[i][j]);
		}
		fprintf(fp, "\n");
	}
	
	fclose(fp);
	
	return 1;
}

int cleanup_list() {
	int i, j, k;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 10; j++) {
			if(strlen(parking_lots[i][j]) <= 2)
				sprintf(((char *)parking_lots[i][j]),"%3d ", atoi(parking_lots[i][j]));
		}
	}
}


int start_process(int c_cnt, int r_cnt){
	int i, j; 
	int cnt_array[2] = {0};
	char buffer[BUFSIZ];
	
	FILE * fp;
	
	fp = fopen("parking_lot.txt", "r");
	
	if(fp == NULL)
		return -1;
	
	while(!feof(fp)){
		fgets(buffer, BUFSIZ, fp);
//		puts(buffer);	
		if(strcmp(buffer, "주차된 차량 목록\n") == 0){
			fscanf(fp, "%d", &c_cnt);	
//			printf("%d\n", c_cnt);
			for(i = 0; i < c_cnt; i++){
				fscanf(fp, "%s%d%d", parked_cars[i].car_num, &(parked_cars[i].parked_place.floor), &(parked_cars[i].parked_place.num));	
//				printf("%s%d%d\n", parked_cars[i].car_num, (parked_cars[i].parked_place.floor), (parked_cars[i].parked_place.num));
			}
		}
		if(strcmp(buffer, "예약된 차량 목록\n") == 0){
			fscanf(fp, "%d", &r_cnt);	
//			printf("%d\n", r_cnt);
			for(i = 0; i < r_cnt; i++){
				fscanf(fp, "%s%d%d", reserved_cars[i].car_num, &(reserved_cars[i].reserved_place.floor), &(reserved_cars[i].reserved_place.num));	
//				printf("%s%d%d\n", reserved_cars[i].car_num, (reserved_cars[i].reserved_place.floor), (reserved_cars[i].reserved_place.num));
			}
		}
		if(strcmp(buffer, "층별 차량 목록\n") == 0){
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 10; j++) {
					fscanf(fp, "%s", parking_lots[i][j]);
				}
				fprintf(fp, "\n");
			}
		}			
	}
	
	cleanup_list();
	//print_parking_lot(0);
	
	fclose(fp);
	
	return 0;
}

int parking(int cnt, int r_cnt) {
	Car car;
	int floor = 1;
	int flag = 1;
	int command = 0; 
	char car_num[5];
	int resv_num;
	printf("> 차번 입력(숫자 4자리) : ");
	scanf("%s", car_num);
	
	if((resv_num = check_reservation(car_num, r_cnt)) >= 0){	
		parked_cars[cnt].parked_place.floor = reserved_cars[resv_num].reserved_place.floor;
//		printf("%d\n",parked_cars[cnt].parked_place.floor);
		parked_cars[cnt].parked_place.num = reserved_cars[resv_num].reserved_place.num;
//		printf("%d\n",parked_cars[cnt].parked_place.num);
		strcpy(parked_cars[cnt].car_num, car_num);
//		printf("%s\n",parked_cars[cnt].car_num);
			
		strcpy(parking_lots[reserved_cars[resv_num].reserved_place.floor][reserved_cars[resv_num].reserved_place.num], parked_cars[cnt].car_num);
			
		printf("@ 지정차량 : B%d층 %d번 주차했습니다.\r\n", reserved_cars[resv_num].reserved_place.floor + 1, reserved_cars[resv_num].reserved_place.num + 1);
			
		cnt++;
			
		return cnt;			
	}
	
	printf("@ 주차 가능 공간 : B1층[%d대] B2층[%d대] B3층[%d대]\r\n\r\n", available_lots(parking_lots[0]), available_lots(parking_lots[1]), available_lots(parking_lots[2])); 
	
	while(1){
		print_parking_lot(floor);
		printf("> 주차 번호 입력(다른층 0, 취소 -1) : ");
		scanf("%d", &command);
		
		if(!command){
			if(floor == 3)
				flag = flag * -1;
			if(floor == 1 && flag == -1)
				flag = flag * -1;
				
			floor = floor + flag;
			continue;
		}
		else if(command < 0){
			return cnt;
		}
		else {
			parked_cars[cnt].parked_place.floor = (floor - 1);
//			printf("%d\n",parked_cars[cnt].parked_place.floor);
			parked_cars[cnt].parked_place.num = (command - 1);
//			printf("%d\n",parked_cars[cnt].parked_place.num);
			strcpy(parked_cars[cnt].car_num, car_num);
//			printf("%s\n",parked_cars[cnt].car_num);
			
			strcpy(parking_lots[(floor - 1)][(command - 1)], parked_cars[cnt].car_num);
			
			cnt++;
			
			return cnt;
		}	
	}
	
}

int un_parking(Car * car_list, int cnt, int r_cnt){
	
	int i = 0, j, k;
	int resv_num;
	char car[BUFSIZ];
	int index = -1;
	
	printf("> 차번 입력(숫자 4자리) : ");
	scanf("%s", car);
//	puts(car);
	for(i = 0; i < cnt; i++){
		if(strcmp(car_list[i].car_num, car) == 0)
		{
			index = i;
			if((resv_num = check_reservation(car, r_cnt)) >= 0 
			&& car_list[i].parked_place.floor == reserved_cars[resv_num].reserved_place.floor
			&& car_list[i].parked_place.num == reserved_cars[resv_num].reserved_place.num){
				strcpy(parking_lots[car_list[i].parked_place.floor][car_list[i].parked_place.num], "resv");	
			}
			else {
				sprintf(parking_lots[car_list[i].parked_place.floor][car_list[i].parked_place.num], "%3d ", car_list[i].parked_place.num + 1);	
			}
			
			for(j = i; j < (cnt - 1); j++){
				car_list[j] = car_list[j + 1];	
			}
			cnt--;
			
			puts("@ 출차되었습니다.");
			return cnt;
		}
	}
	if(index == -1){
		puts("@ 없는 차량 입니다.");
		return cnt;
	}
}

int reservation(int cnt){
	int i = 0;
	int resv_floor, resv_num;
	char car_num[BUFSIZ]; 
	while(1){
		printf("> 층과 번호 입력(종료 0) : ");
		scanf("%d", &resv_floor);
		if(!resv_floor){
			return cnt;
		}
		scanf("%d",&resv_num);
		printf("> 등록차량 번호 입력 : ");
		scanf("%s", car_num);	
		
		reserved_cars[cnt].reserved_place.floor = (resv_floor - 1);
		reserved_cars[cnt].reserved_place.num = (resv_num - 1);
		strcpy(reserved_cars[cnt].car_num, car_num);
		
		strcpy(parking_lots[(resv_floor - 1)][(resv_num - 1)], "resv");
		
		cnt++;
	}
}

int check_reservation(char * car, int r_cnt){
	int i = 0;
	for(i = 0; i < r_cnt; i++){
		puts(reserved_cars[i].car_num);
		if(strcmp(reserved_cars[i].car_num, car) == 0){
			return i;
		}
	}
	return -1;
}

int main() {
	int c_cnt = 0;
	int r_cnt = 0;
	init_parking_lot();
//	print_parking_lot();
	int i = 0;
	
	start_process(c_cnt, r_cnt);
	#if 1
	while (1) {
		switch (main_menu())
		{
		case 0:
			return end_process(c_cnt, r_cnt);
			break;
		case 1:
			c_cnt = parking(c_cnt, r_cnt);
			break;
		case 2:
			c_cnt = un_parking(parked_cars, c_cnt, r_cnt);
			break;
		case 3:
			r_cnt = reservation(r_cnt);
			break;
		case 4:
			print_parking_lot(0);
			break;
		default:
			break;
		}
	}
#endif
	return 0;
}
