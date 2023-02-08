#include <iostream>
#include <thread>
#include <mutex>
struct SharedResource
{
	static const int N = 5;
	SharedResource() :
		counter(0), recMtx()
	{

	}
	//Shared resource and mutex should be kept together
	std::recursive_mutex recMtx;
	int counter;

};


void func3(SharedResource& sharedResource)
{
	std::unique_lock<std::recursive_mutex> lock(sharedResource.recMtx);
	sharedResource.counter++;
	std::cout << "ThreadID " << std::this_thread::get_id() << ", counter: " << sharedResource.counter << std::endl;
	lock.unlock();
}


void func2(SharedResource& sharedResource) {
	std::unique_lock<std::recursive_mutex> lock(sharedResource.recMtx);
	sharedResource.counter++;
	std::cout << "ThreadID " << std::this_thread::get_id() << ", counter: " << sharedResource.counter << std::endl;
	lock.unlock();
	std::this_thread::sleep_for(3ms); //make the current thread fall asleep
	func3(sharedResource);

}
void func1(SharedResource& sharedResource) {
	std::unique_lock<std::recursive_mutex> lock(sharedResource.recMtx);
	sharedResource.counter++;
	std::cout << "ThreadID " << std::this_thread::get_id() << ", counter: " << sharedResource.counter << std::endl;
	func2(sharedResource);
	
}


int main() {

	SharedResource sharedResource;
	std::thread t1(func1, std::ref(sharedResource));
	std::thread t2(func1, std::ref(sharedResource));
	std::thread t3(func1, std::ref(sharedResource));
	std::thread t4(func1, std::ref(sharedResource));
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}