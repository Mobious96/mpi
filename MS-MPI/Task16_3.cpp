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

int FindByValue(int x, vector<int> &table)
{
	for (int i = 0; i < table.size(); i++)
		if (table[i] == x) return i;
	return -1;
}
int FindByValue(int x, vector<Point> &table)
{
	for (int i = 0; i < table.size(); i++)
		if (table[i].x == x) return i;
	return -1;
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
				if (FindByValue(line[i].x, result) != -1) continue;
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

int comparePoint(const void* a, const void* b)
{
	if (((Point*)(a))->x <	((Point*)(b))->x) return -1;
	if (((Point*)(a))->x >	((Point*)(b))->x) return 1;
	if (((Point*)(a))->x == ((Point*)(b))->x) return 0;
}
int compareInt(const void* a, const void* b)
{
	if (*(int*)a	<	*(int*)b) return -1;
	if (*(int*)a	>	*(int*)b) return 1;
	if (*(int*)a	==	*(int*)b) return 0;
}
int comparePointBySegments(const void* a, const void* b)
{
	if (((Point*)(a))->segment <	((Point*)(b))->segment) return -1;
	if (((Point*)(a))->segment >	((Point*)(b))->segment) return 1;
	if (((Point*)(a))->segment ==	((Point*)(b))->segment) return 0;
}
//size - how many segments will generate
//total_segments - stores 
int DataDistr(Point* &line, vector<Point>& global_line, int &size, int &RankSize, int &max, int &min)
{
	srand(time(NULL));
	int root_SegmentsCount;
	int SegmentsCount;
	int segment_id = 0;
	int mod = (size) % RankSize;

	//Calculate root's number of segmetns:
	SegmentsCount = (size) / RankSize + (mod > 0 ? 1 : 0);
	mod--;
	root_SegmentsCount = SegmentsCount;//buffer
	line = new Point[SegmentsCount * 2];
	for (int i = 0; i < SegmentsCount * 2 - 1; i = i + 2)
	{
		Point left(int((rand() % (max - min)) + min), segment_id, 0);
		Point right(int(left.x + rand() % (max - min) + min + 1), segment_id, 1);
		global_line.push_back(left);
		global_line.push_back(right);
		line[i] = left;
		line[i + 1] = right;
		segment_id++;
	}

	//Calculate other number of segments:
	for (int i = 1; i < RankSize; i++)
	{
		SegmentsCount = (size) / RankSize + (mod > 0 ? 1 : 0);
		mod--;
		MPI_Send(&SegmentsCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		for (int j = 0; j < SegmentsCount; j++)
		{
			Point left(int((rand() % (max - min)) + min), segment_id, 0);			 //for easy reading
			Point right(int(left.x + rand() % (max - min) + min + 1), segment_id, 1);//for easy reading
			global_line.push_back(left);
			global_line.push_back(right);
			MPI_Send(&left, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&right, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
			segment_id++;
		}
	}
	return root_SegmentsCount;
}

void Print(vector<Point> &line)
{
	for (int i = 0; i < line.size(); i++)
	{
		line[i].Print(true);
	}
}
void Print(int* line, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << line[i] << " ";
	}
	cout << endl;
}
int FindRight(vector<Point> &line, int segment_id)
{
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i].segment == segment_id)
			if (line[i].side == 1)
				return i;
	}
}

void main(int argc, char** argv)
{
	int rank;
	int RankSize;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &RankSize);
	Point* line;//part of global_line. Unique for each process.
	vector<Point> global_line;//for sync and duplicate deleting; contains all segments
	int SegmentsCount;//# segments for each process
	int size;//total # of segments
	int* gather_solution;//serves for MPI_Gather
	vector<int> Final_Solution;//final minimal solution
	if (rank == 0)
	{
		int max, min;
		max = 10;
		min = 0;
		size = atoi(argv[1]);

		SegmentsCount = DataDistr(line, global_line, size, RankSize, max, min);
	}
	else
	{
		//Synced with DataDistr()::start
		MPI_Recv(&SegmentsCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		line = new Point[SegmentsCount*2];
		for (int i = 0; i < SegmentsCount*2; i++)
		{
			MPI_Recv(&line[i], 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		//Synced with DataDistr()::end
	}

	//Shared section
	qsort(line, SegmentsCount * 2, sizeof(Point), comparePoint);//local sorting
	vector<int> solution = Cover(line, SegmentsCount);//local covering
	int solution_size = solution.size();
	int recv_buff;//Total solution size
	MPI_Reduce(&solution_size, &recv_buff, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//Gather solutions in one
	if (rank == 0)
	{
		//Print(global_line);
		//cout << "Total points before deleting: " << recv_buff << endl;
		gather_solution = new int[recv_buff];
		for (int i = 0; i < solution.size(); i++)//fill gather_solution with root solution
			gather_solution[i] = solution[i];
		for (int i = solution.size(); i < recv_buff; i++)//fill gather_solution with other solutions
			MPI_Recv(&gather_solution[i], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//Print(gather_solution, recv_buff);
	}
	else
	{
		for (int i = 0; i < solution.size(); i++)//fill gather_solution with other solutions
			MPI_Send(&solution[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	//Delete extra points
	if (rank == 0)
	{
		qsort(gather_solution, recv_buff, sizeof(int), compareInt);
		//Print(global_line);
		vector<vector <int> > segments_list;
		segments_list.resize(size);//������� ������� �������� size x size, � ��� �� ����.
		
		for (int i = 0; i < recv_buff; i++)
		{
			for (int j = 0; j < global_line.size()-1; j = j+2)
			{
				if ((gather_solution[i] >= global_line[j].x) && (gather_solution[i] <= global_line[j + 1].x))
				{
					segments_list[global_line[j].segment].push_back(gather_solution[i]);
				}
			}
		}
		/*for (int i = 0; i < size; i++)
		{
			cout << "i:" << i << " ";
			for (int j = 0; j < segments_list[i].size(); j++)
			{
				cout << segments_list[i][j] << " ";
			}
			cout << endl;
		}*/
		for (int i = 0; i < recv_buff; i++)
		{
			bool CanDelete = true;
			for (int j = 0; j < segments_list.size(); j++)
			{
				for (int k = 0; k < segments_list[j].size(); k++)
				{
					if (segments_list[j][k] == gather_solution[i])
						if (segments_list[j].size() == 1)
						{
							CanDelete = false;
							goto NextSolution;
						}
						else
						{
							segments_list[j].erase(segments_list[j].begin()+k);
						}
				}
			}
		NextSolution:
			if (!CanDelete)
			{
				Final_Solution.push_back(gather_solution[i]);
			}
		
		}
		/*for (int i = 0; i < size; i++)
		{
			cout << "i:" << i << " ";
			for (int j = 0; j < segments_list[i].size(); j++)
			{
				cout << segments_list[i][j] << " ";
			}
			cout << endl;
		}*/
		/*cout << "After deliting: " << Final_Solution.size() << endl;
		for (int i = 0; i < Final_Solution.size(); i++)
		{
			cout << Final_Solution[i] << " ";
		}
		cout << endl;*/
	}

	////Delete extra points.
	//if (rank == 0)
	//{
	//	//��������:
	//	//����� ������ ����� �� �������. ������� � � ���������� �����. ���� �����.
	//	//
	//	//
	//	vector<int> segments_solutions(size);
	//	for (int i = 0; i < size; i++)
	//		segments_solutions[i] = 0;

	//	qsort(&global_line[0], global_line.size(), sizeof(Point), comparePoint);
	//	qsort(gather_solution, recv_buff, sizeof(int), compareInt);
	//	Print(gather_solution, recv_buff);
	//	for (int i = recv_buff-1; i >= 0; i--)
	//	{
	//		bool WorthIt = false;
	//		int j = FindByValue(gather_solution[i], global_line);
	//		while (j >= 0)
	//		{		
	//			if (global_line[j].side == 0)
	//			{
	//				int k = FindRight(global_line, global_line[j].segment);
	//				if (global_line[k].x >= gather_solution[i])
	//				{
	//					segments_solutions[global_line[j].segment]++;
	//					if (segments_solutions[global_line[j].segment] == 1)
	//					{
	//						cout << global_line[j].segment << " "<< segments_solutions[global_line[j].segment] << endl;
	//						WorthIt = true;
	//					}
	//						
	//				}
	//			}
	//			j--;
	//		}
	//		if (WorthIt)
	//		{
	//			cout << "pushing " << gather_solution[i] << endl;
	//			Final_Solution.push_back(gather_solution[i]);
	//		}
	//	}
	//	cout << "After deliting: " << Final_Solution.size() << endl;
	//	for (int i = 0; i < Final_Solution.size(); i++)
	//	{
	//		cout << Final_Solution[i] << " ";
	//	}
	//	cout << endl;
	//}
	
	
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