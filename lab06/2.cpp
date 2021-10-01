#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <chrono>
#include <condition_variable>
#include <vector>

using namespace std;

typedef unsigned char _byte;

class Q
{
public:

	Q(unsigned int max_size = UINT_MAX)
	{
		this->_max_size = max_size;
	}

	void push(_byte val)
	{
		unique_lock<mutex> l(mtx);
		if (cont.size() == _max_size)
		{
			cv_pop.wait(l);
		}
		cont.push_back(val);
	}

	bool pop(_byte& val)
	{
		unique_lock<mutex> l(mtx_pop);
		if (cont.empty())
		{
			this_thread::sleep_for(2.5ms);
		}
		if (!cont.empty())
		{
			mtx.lock();
			val = cont.back();
			cont.pop_back();
			mtx.unlock();
			cv_pop.notify_one();
			return true;
		}
		return false;
	}

	void re()
	{
		cont.clear();
	}

	unsigned int len()
	{
		return cont.size();
	}

	unsigned int max_size()
	{
		return _max_size;
	}

	void resize(unsigned int size)
	{
		this->re();
		this->_max_size = size;
	}

	unsigned int size()
	{
		return cont.size();
	}

private:

	vector<_byte> cont;
	unsigned int _max_size;
	condition_variable cv_pop;
	condition_variable cv_push;
	mutex mtx;
	mutex mtx2;
	mutex mtx_pop;
};

int tasksNum = 1 << 12;
int ProducerNum[]{ 1, 2, 4 };
int ConsumerNum[]{ 1, 2, 4 };
int QueueSize[]{ 1, 4, 16 };
Q q;

int consumer();
void producer();

int main()
{
	thread producers[4];
	future<int> consumers[4];

	for (auto qsize : QueueSize)
	{
		q.resize(qsize);

		for (auto pn : ProducerNum)
		{
			for (auto cn : ConsumerNum)
			{
				int sum = 0;

				auto start = chrono::high_resolution_clock::now();

				for (int i = 0; i < pn; i++)
				{
					producers[i] = thread(producer);
					producers[i].detach();
				}
				for (int i = 0; i < cn; i++)
				{
					consumers[i] = async(consumer);
				}
				for (int i = 0; i < cn; i++)
				{
					sum += consumers[i].get();
				}

				auto end = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
				long long count = duration.count();

				cout << "producers: " << pn << "\n"
					<< "consumers: " << cn << "\n"
					<< "sum = " << sum << "\n"
					<< "TaskNum*ProducerNum = " << tasksNum * pn << "\n"
					<< "QueueSize = " << q.max_size() << "\n"
					<< "time: " << count << " ms " << "\n\n";

				q.re();
			}
		}
	}
}

int consumer()
{
	int counter = 0;
	_byte buf = 0;

	while (q.pop(buf))
	{
		counter += buf;
	}
	cout << "consumer done" << "\n";
	return counter;
}

void producer()
{
	for (int i = 0; i < tasksNum; i++)
	{
		q.push(1);
	}
}
