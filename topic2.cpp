#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string.h>

const int MAX_ROW = 100;
const int MAX_COLUMN = MAX_ROW;

void readOneLineData(int* data, char* buffer, int maxCount)
{
    const char* split = ",";
	char *p = strtok(buffer, split);		
    while(p != NULL && maxCount > 0) 
    {
        *data++ = atoi(p);
        p = strtok(NULL, split);
        --maxCount;
    }
}

bool readWeightMatrixDataFromFile(int weight[][MAX_COLUMN], int& rows, int& columns, const char* filePath)
{
    const int MAX_LINE_BUFFER_SIZE = 400;
    const int FIRST_LINE_COUNT = 2;
  
    std::ifstream csvFile(filePath);
    if (!csvFile.is_open())
    {
        std::cout << "Fails to open csv file!" << std::endl;
        return false;
    }
     
    char lineBuffer[MAX_LINE_BUFFER_SIZE];
    if (!csvFile.getline(lineBuffer, MAX_LINE_BUFFER_SIZE)) 
    {
        std::cout << "Fails to read first line!" << std::endl;
        return false;
    }
  
    int firstLine[FIRST_LINE_COUNT];
    readOneLineData(firstLine, lineBuffer, FIRST_LINE_COUNT);
    columns = firstLine[0];
    rows = firstLine[1];
     
    int rowsRead = 0; 
    while (csvFile.getline(lineBuffer, MAX_LINE_BUFFER_SIZE) && rowsRead <rows)
    {
        readOneLineData(&(weight[rowsRead][0]), lineBuffer, columns);
        ++rowsRead;
    } 

    return true;
}

int getMaxTraffic(int rows, int columns, int weight[][MAX_COLUMN])
{    
    // first line
    for (int i = 1; i < columns; ++i)
    {
        weight[0][i] += weight[0][i-1];
    }

    // first column
    for (int i = 1; i < rows; ++i)
    {
        weight[i][0] += weight[i-1][0];
    }
    
    // others
    for (int i = 1; i < rows; ++i)
    {         
        for (int j = 1; j < columns; ++j)
        {
            weight[i][j] += weight[i-1][j] > weight[i][j-1] ? weight[i-1][j] : weight[i][j-1];
        }
    }
  
    return weight[rows-1][columns-1];
}

int main(int argc, char **argv)
{
    int rows, columns;
    int weightMatrix[MAX_ROW][MAX_COLUMN];
    
    if (!readWeightMatrixDataFromFile(weightMatrix, rows, columns, argv[1]))
    {
        return -1;
    }

    std::cout << getMaxTraffic(rows, columns, weightMatrix);

	return 0;		
}
