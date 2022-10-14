#include <iostream>
#include <iomanip>	//setw

using namespace std;

//167: B=3 C=2 S=3 D=3 E=3
//C = 2 - по сумме вклада и дате, К = сумма вклада (ключ поиска)

const int recordsAmount = 4000;	//RECORDS_AMOUNT
const int recordNotFound = -1;	//RECORDS_NOT_FOUND

struct Record
{
	char contributor[30];
	unsigned short int deposit;
	char date[10];
	char lawyer[22];
};

struct Node 
{
    Record record;
    Node *next;
};

Node* open_base_to_list() 
{    
	FILE *database;
	database = fopen("testBase3.dat", "rb");

	Node *recordsList = NULL;

    for (int i = 0; i < recordsAmount; ++i) 
	{
        Record currentRecord;
        fread(&currentRecord, sizeof(Record), 1, database);
        recordsList = new Node {currentRecord, recordsList};
    }
    
	fclose (database);
    return recordsList;
}

void create_index_array(Record** indexArray, Node* recordsList) 
{
    Node *currentRecord = recordsList;
    
    for (int i = 0; i < recordsAmount; ++i) 
	{
        indexArray[i] = &currentRecord->record;
        currentRecord = currentRecord->next;
    }
}

bool compare_two_records(Node* first, Node* second)
{
	if (first->record.deposit == second->record.deposit)
	{
		if(first->record.date[6] == second->record.date[6] && first->record.date[7] == second->record.date[7] &&
		   first->record.date[3] == second->record.date[3] && first->record.date[4] == second->record.date[4] &&
		   first->record.date[0] == second->record.date[0])
		   return first->record.date[1] < second->record.date[1];
		if(first->record.date[6] == second->record.date[6] && first->record.date[7] == second->record.date[7] &&
		   first->record.date[3] == second->record.date[3] && first->record.date[4] == second->record.date[4])
		   return first->record.date[0] < second->record.date[0];
		if(first->record.date[6] == second->record.date[6] && first->record.date[7] == second->record.date[7] &&
		   first->record.date[3] == second->record.date[3])
		   return first->record.date[4] < second->record.date[4];
		if(first->record.date[6] == second->record.date[6] && first->record.date[7] == second->record.date[7])
		   return first->record.date[3] < second->record.date[3];
		if(first->record.date[6] == second->record.date[6])
		   return first->record.date[7] < second->record.date[7];
		return first->record.date[6] < second->record.date[6];	
	}
	return first->record.deposit > second->record.deposit;
}

void merge_sort(Node*& recordsList) 
{
	
	struct List 
	{
	    Node* head;
	    Node* tail;
	};
	
    int seriesSizeFactListFirst, seriesSizeFactListSecond, activeListNumber, elementsInListAmount;
    Node *firstSeries = recordsList, *secondSeries = recordsList->next;
    List listArray[2];
    
    //Расщепление на два
    Node *pointerFirst, *pointerSecond;
    pointerFirst = firstSeries;
    pointerSecond = secondSeries;
    while (pointerSecond != NULL) 
	{
        pointerFirst->next = pointerSecond->next;
        pointerFirst = pointerSecond;
        pointerSecond = pointerSecond->next;
    }
    delete pointerFirst, pointerSecond;
    
    
//    Node *tempListFirst = firstSeries, *tempListSecond = secondSeries;
//    while (tempListFirst != NULL) 
//	{
//        tempListFirst = tempListFirst->next;
//    }
//    while (tempListSecond != NULL) 
//	{
//        tempListSecond = tempListSecond->next;
//    }

	//Сортировка слиянием серий - пока ожидаемый размер серии не превысит число элементов в исходном списке (4000)
	int seriesSizeIdeal = 1;
    while (seriesSizeIdeal < recordsAmount) 
	{
		//Инициализация очередей
        listArray[0].tail = listArray[0].head = NULL;
        listArray[1].tail = listArray[1].head = NULL;
        
        activeListNumber = 0;
        elementsInListAmount = recordsAmount;
        
        //Сортировка - пока есть элементы в списке
        while (elementsInListAmount > 0) 
		{
			//Определяем размер серии для первого списка
            if (elementsInListAmount >= seriesSizeIdeal) 
				seriesSizeFactListFirst = seriesSizeIdeal;
            else seriesSizeFactListFirst = elementsInListAmount;
            elementsInListAmount = elementsInListAmount - seriesSizeFactListFirst;
            //Для второго
            if (elementsInListAmount >= seriesSizeIdeal) 
				seriesSizeFactListSecond = seriesSizeIdeal;
            else seriesSizeFactListSecond = elementsInListAmount;
            elementsInListAmount = elementsInListAmount - seriesSizeFactListSecond;
            
            //Слияние серий
            while (seriesSizeFactListFirst != 0 && seriesSizeFactListSecond != 0) 
			{
				//Сравниваем записи
                if (compare_two_records(firstSeries, secondSeries)) 
				{
					//Перемещаем элемент из первого списка + проверка очереди
                    if (listArray[activeListNumber].tail == NULL) 
					{
                        listArray[activeListNumber].tail = listArray[activeListNumber].head = firstSeries;
                    } 
					else 
					{
                        listArray[activeListNumber].tail->next = firstSeries;
                        listArray[activeListNumber].tail = firstSeries;
                    }
                    firstSeries = firstSeries->next;
                    seriesSizeFactListFirst--;

                } 
				else 
				{
					//Из второго
                    if (listArray[activeListNumber].tail == NULL) 
					{
                        listArray[activeListNumber].tail = listArray[activeListNumber].head = secondSeries;
                    } 
					else 
					{
                        listArray[activeListNumber].tail->next = secondSeries;
                        listArray[activeListNumber].tail = secondSeries;
                    }
                    secondSeries = secondSeries->next;
                    seriesSizeFactListSecond--;

                }
            }
            //То же самое для первого
            while (seriesSizeFactListFirst > 0) 
			{
                if (listArray[activeListNumber].tail == NULL) 
				{
                    listArray[activeListNumber].tail = listArray[activeListNumber].head = firstSeries;
                } 
				else 
				{
                    listArray[activeListNumber].tail->next = firstSeries;
                    listArray[activeListNumber].tail = firstSeries;
                }
                firstSeries = firstSeries->next;
                seriesSizeFactListFirst--;
            }
            //То же самое для второго
            while (seriesSizeFactListSecond > 0) 
			{
                if (listArray[activeListNumber].tail == NULL) 
				{
                    listArray[activeListNumber].tail = listArray[activeListNumber].head = secondSeries;
                } 
				else 
				{
                    listArray[activeListNumber].tail->next = secondSeries;
                    listArray[activeListNumber].tail = secondSeries;
                }
                secondSeries = secondSeries->next;
                seriesSizeFactListSecond--;
            }
            //Поменяли активную очередь
            activeListNumber = 1 - activeListNumber;
        }
        firstSeries = listArray[0].head;
        secondSeries = listArray[1].head;
        seriesSizeIdeal *= 2;
    }
    listArray[0].tail->next = NULL;
    recordsList = listArray[0].head;
}

int binary_search(Record** indexArray, unsigned short int key)
{
	int arrSize = recordsAmount;
	int left = 0, right = arrSize - 1;
	
	while(left < right)
	{
		int middle = (left + right) / 2;		
		if(indexArray[middle]->deposit > key)
			left = middle + 1;
		else right = middle;
	}
	
	if(indexArray[right]->deposit == key)
		return right;
		
	return recordNotFound;
}

void database_navigation(Record** indexArray, int currentKeyRecordsAmount = recordsAmount)
{
	const int oneTimeOutputAmount = 20;
	int shown = 0;
	bool isShow = true;
	while (isShow)
	{
		cout << "  #   Contributor                   Date      Deposit  Lawyer\n";
		for (int i = shown; i < shown + oneTimeOutputAmount; ++i)
		{
			Record* currentRecord = indexArray[i];
			cout << setw(4) << i + 1 << ") " << currentRecord->contributor << " " << currentRecord->date << " " << setw(5) << currentRecord->deposit << "    " << currentRecord->lawyer << endl;
		}
			
		cout << "\nNavigation: \n"
			 << "1: next page	2: previous page	3: jump over 10 pages \n"
			 << "4: jump 10 pages back	5: to beggining		6: to the end \n"
			 << "-------> ";
		
		int input;
		cin >> input;
		cout << endl;
		switch (input)
		{
			case 1:
				shown += oneTimeOutputAmount;
				break;
			case 2:
				shown -= oneTimeOutputAmount;	
				break;
			case 3:
				shown += oneTimeOutputAmount * 10;
				break;
			case 4:
				shown -= oneTimeOutputAmount * 10;
				break;
			case 5:
				shown = 0;
				break;
			case 6:
				shown = currentKeyRecordsAmount - oneTimeOutputAmount;
				break;
			default:
				isShow = false;
				break;
		}
		if (shown >= currentKeyRecordsAmount - oneTimeOutputAmount)
		{
			shown = currentKeyRecordsAmount - oneTimeOutputAmount;
			cout << "-----End of the list!-----\n";
		}
		if (shown < 0)
		{
			shown = 0;
			cout << "-----Beginning of the list!-----\n";
		}
	}
}

void search_page(Record** indexArray, int &currentKeyRecordsAmount)
{
    unsigned short int key;
    cout << "Input key (deposit) ---> ";
    cin >> key;
    
    int firstRecordIndex = binary_search(indexArray, key);
    if (firstRecordIndex == recordNotFound) 
	{
        cout << "Not found\n";
    }
	else 
	{
        int currentRecord = firstRecordIndex;
        while(indexArray[currentRecord]->deposit == key)
		{
			++currentRecord;
			if(currentRecord == recordsAmount)
				break;
		}	
		currentKeyRecordsAmount = currentRecord - firstRecordIndex;
        database_navigation(&indexArray[firstRecordIndex], currentKeyRecordsAmount);
    }
}

void menu_open()
{
	cout << "Welcome to the course work! \n"
		 << "Menu - press the key: \n"
		 << "1. Open database \n"
		 << "2. Open sorted database \n"
		 << "3. Search with key \n"
		 << "4. Make tree and search \n"
		 << "5. Code database \n"
		 << "Type anything to exit \n"
		 << "-------> ";
}

void menu_goto(Record** indexArrayUnSorted, Record** indexArraySorted)
{
	int currentKeyRecordsAmount = recordNotFound;
	
	bool isOpen = true;
	while(isOpen)
	{
		menu_open();
		int choice;
		cin >> choice;
		switch(choice)
		{
			case 1:
				database_navigation(indexArrayUnSorted);
				break;
			case 2:
				database_navigation(indexArraySorted);
				break;
			case 3:
				search_page(indexArraySorted, currentKeyRecordsAmount);
				break;
			case 4: 
				break;
			case 5: 
				break;
			default:
				isOpen = false;
				cout << "Thanks for working!";
				break;
		}
	}
}

int main()
{
	Node* recordsList = open_base_to_list();
	
	Record* indexArrayUnSorted[recordsAmount];
	create_index_array(indexArrayUnSorted, recordsList);
	
	merge_sort(recordsList);
	
	Record* indexArraySorted[recordsAmount];
	create_index_array(indexArraySorted, recordsList);
	
	menu_goto(indexArrayUnSorted, indexArraySorted);
}
