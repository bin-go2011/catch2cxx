// Let Catch provide main():
#define CATCH_CONFIG_MAIN

// practices for C++ Reactive Programming 

#include <catch.hpp>
#include <thread>

void *ptr = nullptr;

void thread_proc(std::string msg) {
    std::cout << "ThreadProc msg:" << msg;
}

void update_data(std::string& data) {
	data = "Update data";
}

class Factorial
{
public:
    long long myFact;
public:
    Factorial() : myFact(1) {}

    void operator() (int number) {
		ptr = this;
        myFact = 1;
        for (int i=1; i<=number; i++) {
            myFact *= i;
        }

		std::cout << "Factorial of " << number << " is " << myFact << std::endl;
    }
};

TEST_CASE("Passing arguments into a thread", "[thread]" ) {
    SECTION("The arguments passed are copied into the thread's internal storage") {
        // callable object
        Factorial fact; 
        std::thread t1(fact, 10);
        t1.join();

        REQUIRE(ptr != &fact);
        REQUIRE(fact.myFact == 1);
    }

    SECTION("multi-threaded Hello World") {
        std::thread t(thread_proc, "Hello World\n");
        t.join();
    }

    SECTION("the stack variables are copied into the thread, safe!") {
        char buf[512];
        const char* hello = "Hello World\n";
        std::strcpy(buf, hello);

        std::thread t(thread_proc, std::string(buf));
        t.detach();
    }

    SECTION("an implicit conversion from char to string may not happen, undefined behavior!!!") {
        char buf[512];
        const char* hello = "Hello World\n";
        std::strcpy(buf, hello);

        std::thread t(thread_proc, buf);
        t.detach();
    }

    SECTION("use the reference to share a data between threads") {
		std::string data = "Hello World\n";
		// std::thread(update_data, data)
		// 1. arguments are simply copied internally
		// 2. refer to the internal copies of arguments and not a reference to the actural parameters
		std::thread t(update_data, std::ref(data));
		t.join();
		REQUIRE(data != "Hello World\n");
    }

}

TEST_CASE("Using Lambdas", "[thread]") {
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread([i]() {
            std::cout << "Thread #" << i << std::endl;
        }));
    }

    std::cout << "\nMain function";

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void function1() 
{ 
    std::cout << "function1()\n"; 
} 
 
void function2() 
{ 
    std::cout << "function2()\n"; 
} 

TEST_CASE("Transfering ownership", "[thread]") {
    std::thread t1(function1); 
     
    // Ownership of t1 is transferred to t2 
    std::thread t2 = std::move(t1);

    t1 = std::thread(function2); 

     // thread instance Created without any associated thread execution 
    std::thread t3; 
     
    // Ownership of t2 is transferred to t3 
    t3 = std::move(t2); 

    // crash !!!
    // the instance t1 is already associated with a running function (function2)
    // std::terminate() is called to terminate the program
    // t1 = std::move(t3);

    // No need to join t2, no longer has any associated thread of execution 
    if (t1.joinable())  t1.join(); 
    if (t3.joinable())  t3.join(); 
}
