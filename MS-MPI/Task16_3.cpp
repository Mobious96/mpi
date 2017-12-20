//�������� ��������/Segments covering

//������ ��� ����� - ����� ��������(��� �����, ��� � ������) � ����������� ��.��� ���� ��� ������ ����� �������� ������ �
//��� ����� �������, � ����� ��, ����� ������ ��� ��� ��������(����� ��� ������).����� ����, ����������� ����� �����
//�������, ���, ���� ���� ��������� ����� � ����� �����������, �� ������� ����� ���� ����� �����, � ������ ����� - ������.
//������ ����, � ������� ����� ��������� ������ ��������, ��������������� � ������ ������; ���������� ���� ����.
//����� ��������� �� ������ � ��������������� �������.���� ������� ����� - ����� �����, �� ������ ��������� ����� � 
//������� � ����.���� �� ��� �������� ������ ������, �� ���������, �� ��� �� ������ ���� �������(��� ����� ����� ������ 
//������� ������ ������� ����������).���� �� ��� ��� ������, �� ������ �� ������ � ��������� � ��������� �����(�������
//�����, �� ����������, ��� � ���� ������ � ����� �������� ������� ��� ���).���� �� �� ��� �� ��� ������, �� ��
//��������� ������� ����� � �����, � ������ �� ����� �������� ��� ���� ������� ��������, ��� ��� ���������� ���������.
//��������� � ����� ��� ��� �������� ������ ���������� ��� ��������, �� ����� ��������� �� ����� �� ������ ������� � 
//��������, ��� �� ��� ������, ���� ���� ��������� �� ��������.�� ��������� ������ ��������� ��� ������� ����� �������,
//� ������ ���������� ������ �����(����������, ����� ����� ����������, ��� ��� ��������� ��������� ������� ���� �����
//�����, � ������ ����� ������).
//
//����� �������, ���� �������� ����������� �� O(N), �� ������ ���������� �����, � �������� ��������� ��������� ��� 
//��� ����� O(N log N).
//http://e-maxx.ru/algo/covering_segments

#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <time.h>
#include <cstdlib>
#include <algorithm>

using namespace std;
struct Point
{
	int x;
	int segment; //the point belongs to segment � %int segment%
	//0 for left segment end
	//1 for right segment end
	int side;

	Point() {};
	Point(int _x, int _segment, int _side)
	{
		x = _x;
		segment = _segment;
		side = _side;
	}
	Point(Point* _p)
	{
		x = _p->x;
		segment = _p->segment;
		side = _p->side;
	}
	Point& operator=(Point* _p)
	{
		x = _p->x;
		segment = _p->segment;
		side = _p->side;
		return *this;
	}
	void Print(bool full_information)
	{
		cout << x;
		if (full_information)
		{
			cout << "\t" << segment;
			cout << "\t" << (side == 0 ? "left" : "right");
		}
		cout << endl;
	}
};
//�������� � ���, ��� ���������� ����� � ���������� ����������� �� ������, ����� ����� ��� �� �������.
//�������, �������������� ����� ���������� ��������� �� ��������, � ����� �����.
//�� � ����� ������ ������� ������ ����������� � ����.
//�����, ���� ���������� ����������, �� ����� ����� ��������� ��� �����, �������������� ����-������ � ��� � ���� ��������.
struct Segment //������, �������� ������� ���������� ��� ��������� ��������� �����.
{
	pair<Point, Point> points;

	Segment() {};
	Segment(Point left, Point right)//Points have to have identical Point.segment and different Point.side fields.
	{
		points.first = left;
		points.second = right;
	}
};
Segment* Generate(int size, int min, int max)
{
	srand(time(NULL));
	Segment* line = new Segment[size];
	for (int i = 0; i < size; i++)
	{
		Point left(int((rand() % (max - min)) + min), i, 0);
		Point right(int(left.x + rand() % (max - min) + min + 1), i, 1);
		line[i].points.first = left;
		line[i].points.second = right;
	}
	return line;
}


vector<int> Cover(Point* &line, int segments_count)
{
	vector<int> result;
	vector<int> marked;//Contains all � of marked segments;
	stack<int> unmarked;//Gives faster marking alghorithm
	for (int i = 0; i < segments_count*2; i++)
	{
		if (line[i].side == 0)//If point is a left segment end
			unmarked.push(line[i].segment);
		else//If point is a right segment end
		{
			bool IsMarked = false;//Was this segment marked before?
			for (int j = 0; j < marked.size(); j++)
			{
				if (marked[j] == line[i].segment)
				{
					IsMarked = true;
					break;
				}
			}
			if (!IsMarked)
			{
				result.push_back(line[i].x);//Add point to the solution
				for (int j = 0; j < unmarked.size(); j++)
				{
					marked.push_back(unmarked.top());//And mark the segment
					unmarked.pop();
				}
			}
		}
	}

	return result;
}

int compare(const void* a, const void* b)
{
	if (((Point*)(a))->x < ((Point*)(b))->x) return -1;
	if (((Point*)(a))->x > ((Point*)(b))->x) return 1;
	if (((Point*)(a))->x == ((Point*)(b))->x) return 0;
}

void main(int argc, char** argv)
{
	int rank;
	int RankSize;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &RankSize);
	Point* line;
	int SegmentsCount;
	if (rank == 0)
	{
		int max, min, size;
		int root_database;
		max = 10;
		min = 0;
		size = atoi(argv[1]);//Segments count
		srand(time(NULL));

		int segments_count = 0;//Two points have to belong to same segment. This var is for generating method below;
		int mod = (size) % RankSize;
		//Calculate root's number of segmetns:
		SegmentsCount = (size) / RankSize + (mod > 0 ? 1 : 0);
		mod--;
		root_database = SegmentsCount;//buffer
		line = new Point[SegmentsCount*2];
		for (int i = 0; i < SegmentsCount*2-1; i = i+2)
		{
			Point left(int((rand() % (max - min)) + min), segments_count, 0);
			Point right(int(left.x + rand() % (max - min) + min + 1), segments_count, 1);
			line[i] = left;
			line[i + 1] = right;
			segments_count++;
		}
		//Calculate other number of segments:
		for (int i = 1; i < RankSize; i++)
		{
			SegmentsCount = (size) / RankSize + (mod > 0 ? 1 : 0);
			mod--;
			MPI_Send(&SegmentsCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			for (int j = 0; j < SegmentsCount; j++)
			{
				Point left(int((rand() % (max - min)) + min), segments_count, 0);
				Point right(int(left.x + rand() % (max - min) + min + 1), segments_count, 1);
				MPI_Send(&left, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&right, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
				segments_count++;
			}
		}
		SegmentsCount = root_database;
	}
	else
	{
		MPI_Recv(&SegmentsCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		line = new Point[SegmentsCount*2];
		for (int i = 0; i < SegmentsCount*2-1; i = i+2)
		{
			MPI_Recv(&line[i], 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&line[i + 1], 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	qsort(line, SegmentsCount * 2, sizeof(Point), compare);
	vector<int> solution = Cover(line, SegmentsCount);

	/*if (rank == 1)
	{
		for (int i = 0; i < SegmentsCount * 2; i++)
		{
			line[i].Print(true);
		}
		cout << endl;
		qsort(line, SegmentsCount * 2, sizeof(Point), compare);
		for (int i = 0; i < SegmentsCount * 2; i++)
		{
			line[i].Print(true);
		}
	}*/
	
	delete[] line;
	//Segment* line_raw;
	//Point* line_sorted;

	//int* Displ;
	//int* Sendcounts;
	//int DataSize;
	//if (rank == 0)
	//{
	//	int size = 3;
	//	line_raw = Generate(size, 0, 10);
	//	DataSize = DataDistr(Displ, Sendcounts, size * 2, RankSize);
	//}
	//else
	//{
	//	MPI_Recv(&DataSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//	line_raw = new Segment[DataSize];
	//	for (int i = 0; i < DataSize; i++)
	//	{
	//		MPI_Recv(&line_raw[i].points.first, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//		MPI_Recv(&line_raw[i].points.second, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//	}
	//}





	/*Point a;
	if (rank == 0)
	{
		a.x = 1; a.segment = 2; a.side = 0;
		MPI_Send(&a, 3, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&a, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << a.x << " " << a.segment << " " << a.side << endl;
	}*/

	/*delete[] line;*/
	MPI_Finalize();
}