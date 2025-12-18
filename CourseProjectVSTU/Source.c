#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCH 2.54

typedef struct monitor {
	int id;
	char manufacturer[31];
	float diagonal;
	unsigned int horizontal_resolution;
	unsigned int vertical_resolution;
	char panel_type[20];
	unsigned short int curved;
	char hdmi_port[50];
}Monitor;


Monitor* searchMonitorDiagonal(Monitor monitors[], int size, float search_element, int* result_count);
Monitor* searchMonitorPanelType(Monitor monitors[], int size, char search_element[], int* result_count);
Monitor* searchMonitorDiagonalAndPanelType(Monitor monitors[], int size, float search_diagonal, char search_panel_type[], int* result_count);

Monitor* addData(Monitor monitors[], int* size);

int getManufacturer(Monitor* monitors, int index);

unsigned short int isBinaryInput();
unsigned int getResolution();

float getDiagonal();

int fillMonitorArray(Monitor monitors[], int size);
int printMonitorArray(Monitor monitors[], int size, int category_of_measurement);
int printInFileMonitorArray(FILE* stream, Monitor monitors[], int size, int category_of_measurement);
int changeMeasurement(Monitor monitors[], int size, int* category_of_measurement);
int compareManufacturer(const void* a, const void* b);
int compareResolution(const void* a, const void* b);
int compareManufacturerAndResolution(const void* a, const void* b);
int changeData(Monitor monitors[], int size);
int editID(Monitor* monitors, int size);

int input_file(char* filename, Monitor* monitors);
int output_file(char* filename, Monitor* monitors, int size, int* category_of_measurement);

Monitor* get_Input_File(char* filename, Monitor* monitors, int* size);

int main(void) {
	setlocale(LC_CTYPE, "RUS");

	Monitor* monitors = NULL;
	Monitor* searcher;

	char a;
	char fname[30];

	int category_of_measurement = 0;
	int clear;
	int size;
	int menu_active = 1;
	int search_count = 0;

	float search_diagonal;


	while (menu_active) {
		printf("Выберите действие: \n"
			"0. Заполнить массив тестовыми значениями \n"
			"1. Заполнить массив с пользователем \n"
			"2. Загрузка данных из файла \n");

		scanf(" %c", &a);
		while ((clear = getchar()) != '\n');

		switch (a)
		{
		case '0':
			size = 5;
			monitors = (Monitor*)malloc(size * sizeof(Monitor));
			testMonitorArray(monitors, size);
			printMonitorArray(monitors, size, category_of_measurement);
			menu_active = 0;
			break;
		case '1':
			printf("Введите количество записей: ");
			scanf("%d", &size);
			monitors = (Monitor*)malloc(size * sizeof(Monitor));
			fillMonitorArray(monitors, size);
			printMonitorArray(monitors, size, category_of_measurement);
			menu_active = 0;
			break;
		case '2':
			printf("Имя файла: ");
			scanf("%s", fname);
			if (monitors != NULL)
				free(monitors);
			monitors = get_Input_File(fname, monitors, &size);
			if (monitors != NULL) {
				printMonitorArray(monitors, size, category_of_measurement);
				menu_active = 0;
			}
			break;
		default:
			puts("Ошибка выбора! Повторите попытку");
			break;
		}
	}

	menu_active = 1;
	while (menu_active) {
		int submenu_active;
		printf("=== Главное меню === \n"
			"1. Поиск и сортировка \n"
			"2. Работа с данными \n"
			"3. Работа с файлами \n"
			"q. Завершить работу \n");

		//while (getchar() != '\n');
		//a = getchar();
		scanf(" %c", &a);
		while ((clear = getchar()) != '\n');


		switch (a)
		{
		case '1':
			submenu_active = 1;
			break;
		case '2':
			submenu_active = 2;
			break;
		case '3':
			submenu_active = 3;
			break;
		case 'q':
			return 0;
		default:
			puts("Ошибка выбора! Повторите попытку");
			continue;
		}



		if (submenu_active == 1) {
			char search_panel_type[20];
			float search_diagonal;
			while (submenu_active) {
				printf("Выберите действие: \n"
					"1. Поиск по диагонали \n"
					"2. Поиск по матрице \n"
					"3. Поиск по диагонали и матрице \n"
					"4. Сортировка по производителю \n"
					"5. Сортировка по разрешению \n"
					"6. Сортировка по производетелю и разрешению \n"
					"b. Главное меню \n");

				scanf(" %c", &a);
				while ((clear = getchar()) != '\n');

				switch (a)
				{
				case '1':
					printf("Выберите единицу измерения (0 - дюймы, 1 - сантиметры): ");
					unsigned short int check_category = isBinaryInput();
					if(category_of_measurement == check_category)
						printMonitorArray(monitors, size, category_of_measurement);
					else {
						changeMeasurement(monitors, size, &category_of_measurement);
						printMonitorArray(monitors, size, category_of_measurement);
					}

					printf("Искомый элемент: ");
					scanf("%f", &search_diagonal);
					searcher = searchMonitorDiagonal(monitors, size, search_diagonal, &search_count);
					printf("Найдено элементов: %d \n", search_count);
					printMonitorArray(searcher, search_count, category_of_measurement);
					break;
				case '2':
					printf("Искомый элемент: ");
					scanf("%s", search_panel_type);
					searcher = searchMonitorPanelType(monitors, size, search_panel_type, &search_count);
					printf("Найдено элементов: %d \n", search_count);
					printMonitorArray(searcher, search_count, category_of_measurement);
					break;
				case '3':
					printf("Искомая диагональ: ");
					scanf("%f", &search_diagonal);
					printf("Искомая матрица: ");
					scanf("%s", search_panel_type);
					searcher = searchMonitorDiagonalAndPanelType(monitors, size, search_diagonal, search_panel_type, &search_count);
					printf("Найдено элементов: %d \n", search_count);
					printMonitorArray(searcher, search_count, category_of_measurement);
					break;
				case '4':
					printf("Отсортированный массив: \n");
					qsort(monitors, size, sizeof(Monitor), compareManufacturer);
					editID(monitors, size);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case '5':
					printf("Отсортированный массив: \n");
					qsort(monitors, size, sizeof(Monitor), compareResolution);
					editID(monitors, size);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case '6':
					printf("Отсортированный массив: \n");
					qsort(monitors, size, sizeof(Monitor), compareManufacturerAndResolution);
					editID(monitors, size);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case 'b':
					submenu_active = 0;
					break;
				default:
					puts("Ошибка выбора! Повторите попытку");
					break;
				}
			}
		}
		else if (submenu_active == 2) {
			while (submenu_active) {
				printf("Выберите действие: \n"
					"1. Изменить единицу измерения \n"
					"2. Добавить элементы \n"
					"3. Изменить данные \n"
					"b. Главное меню \n");

				scanf(" %c", &a);
				while ((clear = getchar()) != '\n');

				switch (a)
				{
				case '1':
					changeMeasurement(monitors, size, &category_of_measurement);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case '2':
					monitors = addData(monitors, &size);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case '3':
					changeData(monitors, size);
					printMonitorArray(monitors, size, category_of_measurement);
					break;
				case 'b':
					submenu_active = 0;
					break;
				default:
					puts("Ошибка выбора! Повторите попытку");
					break;
				}
			}
		}
		else if (submenu_active == 3) {
			while (submenu_active) {
				printf("Выберите действие: \n"
					"1. Выгрузить в файл \n"
					"2. Загрузить из файла \n"
					"3. Посмотреть содержимое файла \n"
					"b. Главное меню \n");

				scanf(" %c", &a);
				while ((clear = getchar()) != '\n');

				if (a == 'b') break;

				printf("Имя файла: ");
				scanf("%s", fname);
				switch (a)
				{
				case '1':
					output_file(fname, monitors, size, &category_of_measurement);
					break;
				case '2':
					if (monitors != NULL)
						free(monitors);
					monitors = get_Input_File(fname, monitors, &size);
					if (monitors != NULL)
						printMonitorArray(monitors, size, category_of_measurement);
					break;
				case '3':
					input_file(fname, monitors);
					break;
				default:
					puts("Ошибка выбора! Повторите попытку");
					break;
				}
			}
		}

		
	}

	system("pause");
	return 0;
}

Monitor* get_Input_File(char* filename, Monitor* monitors, int* size) {
	char buffer[256];
	int count = -1;
	Monitor* temp;
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Ошибка открытия файла\n");
		return NULL;
	}

	//for (int i = 0; i < 2; i++) {
	//	if (fgets(buffer, sizeof(buffer), file) == NULL) {
	//		puts("Недостаточно данных");
	//		fclose(file);
	//		return NULL;
	//	}
	//}

	while (fgets(buffer, sizeof(buffer), file)) {
		if (buffer[0] != ' '/* || buffer[0] == '\n' || buffer[0] == '\0'*/)
			continue;
		count++;
	}


	rewind(file);

	for (int i = 0; i < 2; i++) {
		if (fgets(buffer, sizeof(buffer), file) == NULL) {
			puts("Недостаточно данных");
			fclose(file);
			return NULL;
		}
	}


	temp = (Monitor*)malloc(count * sizeof(Monitor));
	for (int i = 0; i < count; i++) {
		if (fgets(buffer, sizeof(buffer), file) == NULL) continue;

		int j = 0;

		if (buffer[0] != ' '/* || buffer[0] == '\n' || buffer[0] == '\0'*/)
			continue;

		for (int k = 0; k < sizeof(buffer); k++) {
			if (buffer[k] != ' ' && buffer[k] != '\n') {
				buffer[j] = buffer[k];
				j++;
			}


		}

		buffer[j - 1] = '\0';



		sscanf(buffer, "%*d||%29[^|]||%f||%ux%u||%19[^|]||%hu||%20[^|]",
			temp[i].manufacturer,
			&temp[i].diagonal,
			&temp[i].horizontal_resolution, &temp[i].vertical_resolution,
			temp[i].panel_type,
			&temp[i].curved,
			temp[i].hdmi_port);
		temp[i].id = i;
	}

	//printMonitorArray(monitors, count, 1);
	printf("=== Количество элементов в массиве: %d ===\n", count);

	//while (!feof(file)) {
	//	fscanf(file, "")
	//}
	//for (int i = 0; i < count; i++)
	//{
	//	fscanf(file, "   %d  ||           %s ||         %f ||  3840x2160 ||        OLED ||           1 ||          2x HDMI 2.1 ")
	//}
	*size = count;
	fclose(file);

	return temp;

}

Monitor* addData(Monitor monitors[], int* size) {
	Monitor* newCountOfElements;

	int number_of_elements = 0;
	printf("Количество добавляемых элементов: ");
	while (1) {
		scanf("%d", &number_of_elements);
		if (number_of_elements < 0)
			printf("Ошибка! Число не может быть отрицательным. Введите заново: ");
		else break;
	}
	number_of_elements += *size;

	newCountOfElements = (Monitor*)malloc(number_of_elements * sizeof(Monitor));

	for (int i = 0; i < *size; i++)
		newCountOfElements[i] = monitors[i];

	for (int i = *size; i < number_of_elements; i++) {
			//if(monitors[i].id == NULL)
		newCountOfElements[i].id = i;

		printf("Заполнение карточки элемента [%d] \n", i);
		getManufacturer(newCountOfElements, i);

		printf("Диагональ: ");
		newCountOfElements[i].diagonal = getDiagonal();

		printf("Горизонтальное разрешение: ");
		newCountOfElements[i].horizontal_resolution = getResolution();
		printf("Вертикальное разрешение: ");
		newCountOfElements[i].vertical_resolution = getResolution();

		printf("Матрица: ");
		scanf("%s", newCountOfElements[i].panel_type);

		printf("Изогнутость (1 - есть, 0 - нет): ");
		newCountOfElements[i].curved = isBinaryInput();

		printf("HDMI-порт: ");
		scanf("%s", newCountOfElements[i].hdmi_port);
	}

	*size = number_of_elements;

	return newCountOfElements;
}

int editID(Monitor* monitors, int size) {
	for (int i = 0; i < size; i++)
		monitors[i].id = i;
	return 0;
}

int changeData(Monitor monitors[], int size) {
	int id;
	int choice;

	do {
		printf("Введите номер элемента (0-%d): ", size - 1);
		do {
			scanf("%d", &id);
			while (getchar() != '\n');

			if (id < 0 || id >= size) {
				printf("Ошибка! Введите число от 0 до %d: ", size - 1);
			}
		} while (id < 0 || id >= size);

		printf("Заполнение карточки элемента [%d] \n", id);
		getManufacturer(monitors, id);

		printf("Диагональ: ");
		monitors[id].diagonal = getDiagonal();

		printf("Горизонтальное разрешение: ");
		monitors[id].horizontal_resolution = getResolution(); 
		printf("Вертикальное разрешение: ");
		monitors[id].vertical_resolution = getResolution();

		printf("Матрица: ");
		scanf("%s", monitors[id].panel_type);

		printf("Изогнутость (1 - есть, 0 - нет): ");
		monitors[id].curved = isBinaryInput();

		printf("HDMI-порт: ");
		scanf("%s", monitors[id].hdmi_port);

		printf("Введите любой символ, чтобы продолжить. 0 - прекратить изменения: ");
		scanf("%d", &choice);

	} while (choice);

	return 0;
}

unsigned int getResolution() {
	int value;
	while (1) {
		if ((scanf("%d", &value)) == 1 && value > -1 && value < 10000) {
			while (getchar() != '\n');
			return value;
		}
		else {
			while (getchar() != '\n');
			printf("Ошибка! Некорректное значение. Введите заново: ");
		}
	}
}

int getManufacturer(Monitor* monitors, int index) {
	char manufacturer[31];

	printf("Производитель (максимум 30 символов): ");
	while (1) {
		int count = 0;
		int symbols = 0;

		scanf("%30s", manufacturer);
		while (getchar() != '\n');

		for (int i = 0; manufacturer[i] != '\0'; i++) {
			symbols++;
			if (manufacturer[i] != '1' && manufacturer[i] != '2' && manufacturer[i] != '3' && manufacturer[i] != '4' && manufacturer[i] != '5' && manufacturer[i] != '6' && manufacturer[i] != '7' && manufacturer[i] != '8' && manufacturer[i] != '9' && manufacturer[i] != '0')
				count++;
		}

		if (count == symbols) {
			strcpy(monitors[index].manufacturer, manufacturer);
			return 0;
		}

		printf("Производитель не должен содержать в названии число! Повторите попытку: ");
	}
}

float getDiagonal() {
	float value;
	while (1) {
		scanf("%f", &value);
		if (value > -1) {
			while (getchar() != '\n');
			return value;
		}
		else {
			while (getchar() != '\n');
			printf("Ошибка! Некорректное значение. Введите заново: ");
		}
	}
}

unsigned short int isBinaryInput() {
	unsigned short int value;
	while (1) {
		scanf("%hu", &value);
		if (value == 0 || value == 1) {
			while (getchar() != '\n');
			return value;
		}
		else {
			while (getchar() != '\n');
			printf("Ошибка! Некорректное число. Введите заново: ");
		}
	}
}

int input_file(char* filename, Monitor* monitors) {
	char buffer[256];
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		puts("Ошибка открытия файла");
		return 1;
	}
	while (fgets(buffer, sizeof(buffer), file)) {
		printf(buffer);
	}

	fclose(file);
}

int output_file(char* filename, Monitor* monitors, int size, int* category_of_measurement) {
	if (*category_of_measurement == 1) {
		*category_of_measurement = 0;
		changeMeasurement(monitors, size, &category_of_measurement);
	}
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		puts("Ошибка открытия файла");
		return 1;
	}

	printInFileMonitorArray(file, monitors, size, category_of_measurement);

	fclose(file);

	puts("=== Данные успешно выгружены ===");

	return 0;
}

int changeMeasurement(Monitor monitors[], int size,  int* category_of_measurement) {
	if (*category_of_measurement == 0)
		*category_of_measurement = 1;
	else
		*category_of_measurement = 0;
	for (int i = 0; i < size; i++) {
		if (*category_of_measurement)
			monitors[i].diagonal *= INCH;
		else
			monitors[i].diagonal /= INCH;
	}
	return 0;
}

int fillMonitorArray(Monitor monitors[], int size) {
	for (int i = 0; i < size; i++) {
		//if(monitors[i].id == NULL)
			monitors[i].id = i;

		printf("Заполнение карточки элемента [%d] \n", i);
		//printf("Производитель: ");
		//scanf("%s", monitors[i].manufacturer);
		getManufacturer(monitors, i);

		printf("Диагональ: ");
		monitors[i].diagonal = getDiagonal();

		printf("Горизонтальное разрешение: ");
		monitors[i].horizontal_resolution = getResolution();
		printf("Вертикальное разрешение: ");
		monitors[i].vertical_resolution = getResolution();

		printf("Матрица: ");
		scanf("%s", monitors[i].panel_type);

		printf("Изогнутость (1 - есть, 0 - нет): ");
		monitors[i].curved = isBinaryInput();

		printf("HDMI-порт: ");
		scanf("%s", monitors[i].hdmi_port);
	}
	return 0;
}

int testMonitorArray(Monitor monitors[], int size) {
	monitors[0].id = 0;
	strcpy(monitors[0].manufacturer, "Dell");
	monitors[0].diagonal = 27.0f;
	monitors[0].horizontal_resolution = 2560;
	monitors[0].vertical_resolution = 1440;
	strcpy(monitors[0].panel_type, "IPS");
	monitors[0].curved = 0;
	strcpy(monitors[0].hdmi_port, "1x_HDMI_2.0");

	monitors[1].id = 1;
	strcpy(monitors[1].manufacturer, "Samsung");
	monitors[1].diagonal = 34.0f;
	monitors[1].horizontal_resolution = 3440;
	monitors[1].vertical_resolution = 1440;
	strcpy(monitors[1].panel_type, "VA");
	monitors[1].curved = 1;
	strcpy(monitors[1].hdmi_port, "2x_HDMI_2.1");

	monitors[2].id = 2;
	strcpy(monitors[2].manufacturer, "LG");
	monitors[2].diagonal = 24.0f;
	monitors[2].horizontal_resolution = 1920;
	monitors[2].vertical_resolution = 1080;
	strcpy(monitors[2].panel_type, "IPS");
	monitors[2].curved = 0;
	strcpy(monitors[2].hdmi_port, "2x_HDMI_1.4");

	monitors[3].id = 3;
	strcpy(monitors[3].manufacturer, "ASUS");
	monitors[3].diagonal = 32.0f;
	monitors[3].horizontal_resolution = 3840;
	monitors[3].vertical_resolution = 2160;
	strcpy(monitors[3].panel_type, "OLED");
	monitors[3].curved = 1;
	strcpy(monitors[3].hdmi_port, "2x_HDMI_2.1");

	monitors[4].id = 4;
	strcpy(monitors[4].manufacturer, "Dell");
	monitors[4].diagonal = 27.0f;
	monitors[4].horizontal_resolution = 1920;
	monitors[4].vertical_resolution = 1080;
	strcpy(monitors[4].panel_type, "TN");
	monitors[4].curved = 0;
	strcpy(monitors[4].hdmi_port, "1x_VGA");

	return 0;
}

int printInFileMonitorArray(FILE* stream, Monitor monitors[], int size, int category_of_measurement) {
	/*if (category_of_measurement)*/
		fputs("  id  ||  Производитель || Диагональ (дюймы) || Разрешение ||   Матрица   || Изогнутость ||         HDMI-порт         |\n", stream);
	//else
	//	fputs("  id  ||  производитель || Диагональ (дюймы) || Разрешение ||   Матрица   || Изогнутость || HDMI-порт |\n", stream);
	fputs("-----------------------------------------------------------------------------------------------------------------------\n", stream);
	for (int i = 0; i < size; i++) {
		fprintf(stream, " %3d  || %14s || %17.2f || %5ux%4u || %11s || %11hu || %25s |\n",
			monitors[i].id,
			monitors[i].manufacturer,
			monitors[i].diagonal,
			monitors[i].horizontal_resolution, monitors[i].vertical_resolution,
			monitors[i].panel_type,
			monitors[i].curved,
			monitors[i].hdmi_port);
	}
	return 0;
}

int printMonitorArray(Monitor monitors[], int size, int category_of_measurement) {
	char buffer[256];
	char label_inch[] = "  id  ||  Производитель || Диагональ (дюймы) || Разрешение ||   Матрица   || Изогнутость ||         HDMI-порт         |";
	char label_cm[] = "  id  ||  Производитель ||   Диагональ (см)  || Разрешение ||   Матрица   || Изогнутость ||         HDMI-порт         |";
	char info[] = " %3d  || %14s || %17.2f || %5ux%4u || %11s || %11hu || %25s |";
	
	if (category_of_measurement)
		puts(label_cm);
	else
		puts(label_inch);
	puts("-----------------------------------------------------------------------------------------------------------------------");
	for (int i = 0; i < size; i++) {
		sprintf(buffer, info,
			monitors[i].id,
			monitors[i].manufacturer,
			monitors[i].diagonal,
			monitors[i].horizontal_resolution, monitors[i].vertical_resolution,
			monitors[i].panel_type,
			monitors[i].curved,
			monitors[i].hdmi_port);
		puts(buffer);
	}

	return 0;
}

Monitor* searchMonitorDiagonalAndPanelType(Monitor monitors[], int size, float search_diagonal, char search_panel_type[], int* result_count) {
	int count = 0;
	Monitor* search_result_array;
	for (int i = 0; i < size; i++)
	{
		if (monitors[i].diagonal == search_diagonal && strcmp(monitors[i].panel_type, search_panel_type) == 0)
			count++;
	}
	search_result_array = (Monitor*)malloc(count * sizeof(Monitor));
	*result_count = count;
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		if (monitors[i].diagonal == search_diagonal && strcmp(monitors[i].panel_type, search_panel_type) == 0) {
			search_result_array[j].id = monitors[i].id;
			strcpy(search_result_array[j].manufacturer, monitors[i].manufacturer);
			search_result_array[j].diagonal = monitors[i].diagonal;
			search_result_array[j].horizontal_resolution = monitors[i].horizontal_resolution;
			search_result_array[j].vertical_resolution = monitors[i].vertical_resolution;
			strcpy(search_result_array[j].panel_type, monitors[i].panel_type);
			search_result_array[j].curved = monitors[i].curved;
			strcpy(search_result_array[j].hdmi_port, monitors[i].hdmi_port);
			j++;
		}
	}
	return search_result_array;
}

Monitor* searchMonitorDiagonal(Monitor monitors[], int size, float search_element, int* result_count) {
	int count = 0;
	Monitor* search_result_array;
	for (int i = 0; i < size; i++)
	{
		if (monitors[i].diagonal == search_element)
			count++;
	}
	search_result_array = (Monitor*)malloc(count * sizeof(Monitor));
	*result_count = count;
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		if (monitors[i].diagonal == search_element) {
			search_result_array[j].id = monitors[i].id;
			strcpy(search_result_array[j].manufacturer, monitors[i].manufacturer);
			search_result_array[j].diagonal = monitors[i].diagonal;
			search_result_array[j].horizontal_resolution = monitors[i].horizontal_resolution;
			search_result_array[j].vertical_resolution = monitors[i].vertical_resolution;
			strcpy(search_result_array[j].panel_type, monitors[i].panel_type);
			search_result_array[j].curved = monitors[i].curved;
			strcpy(search_result_array[j].hdmi_port, monitors[i].hdmi_port);
			j++;
		}
	}
	return search_result_array;
}

Monitor* searchMonitorPanelType(Monitor monitors[], int size, char search_element[], int* result_count) {
	int count = 0;
	Monitor* search_result_array;
	for (int i = 0; i < size; i++)
	{
		if (strcmp(monitors[i].panel_type, search_element) == 0)
			count++;
	}
	search_result_array = (Monitor*)malloc(count * sizeof(Monitor));
	*result_count = count;
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		if (strcmp(monitors[i].panel_type, search_element) == 0) {
			search_result_array[j].id = monitors[i].id;
			strcpy(search_result_array[j].manufacturer, monitors[i].manufacturer);
			search_result_array[j].diagonal = monitors[i].diagonal;
			search_result_array[j].horizontal_resolution = monitors[i].horizontal_resolution;
			search_result_array[j].vertical_resolution = monitors[i].vertical_resolution;
			strcpy(search_result_array[j].panel_type, monitors[i].panel_type);
			search_result_array[j].curved = monitors[i].curved;
			strcpy(search_result_array[j].hdmi_port, monitors[i].hdmi_port);
			j++;
		}
	}
	return search_result_array;
}

int compareManufacturer(const void* a, const void* b) {

	const Monitor* MonitorA = (const Monitor*)a;
	const Monitor* MonitorB = (const Monitor*)b;


	return _stricmp(MonitorA->manufacturer, MonitorB->manufacturer);



	//return strcmp(MonitorA->manufacturer, MonitorB->manufacturer);
}

int compareResolution(const void* a, const void* b) {

	const Monitor* MonitorA = (const Monitor*)a;
	const Monitor* MonitorB = (const Monitor*)b;

	int resolutionA = MonitorA->horizontal_resolution * MonitorA->vertical_resolution;
	int resolutionB = MonitorB->horizontal_resolution * MonitorB->vertical_resolution;

	if (resolutionA < resolutionB)
		return -1;
	else if (resolutionA > resolutionB)
		return 1;

	return 0;
}

int compareManufacturerAndResolution(const void* a, const void* b) {

	const Monitor* monitorA = (const Monitor*)a;
	const Monitor* monitorB = (const Monitor*)b;

	int manufacturer_cmp = _stricmp(monitorA->manufacturer, monitorB->manufacturer);
	if (manufacturer_cmp != 0)
		return manufacturer_cmp;

	int resolutionA = monitorA->horizontal_resolution * monitorA->vertical_resolution;
	int resolutionB = monitorB->horizontal_resolution * monitorB->vertical_resolution;

	if (resolutionA < resolutionB)
		return -1;
	else if (resolutionA > resolutionB)
		return 1;

	return 0;
}

