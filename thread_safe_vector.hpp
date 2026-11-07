// thread_safe_vector.hpp: starter code -- please refer to in-class example for ThreadSafeQueue
#include <mutex>
#include <condition_variable>
#include <vector>
#include <iterator>

template<typename T>
class ThreadSafeVector
{
public:
    void push_back(const T& value);
    bool empty() const
    std::size_t size() const;
	bool try_pop_back(T& popped_value);
	void wait_and_pop_back(T& popped_value);
	bool try_at(std::size_t index, T& value) const;
	void clear();

private:
    std::vector<T> the_vector;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

template<typename T>
bool ThreadSafeVector<T>::empty() const
{
	std::lock_guard<std::mutex> lock(the_mutex);

	return the_vector.empty();
}

template<typename T>
std::size_t ThreadSafeVector<T>::size() const
{
	std::lock_guard<std::mutex> lock(the_mutex);

	return the_vector.size();
}

template<typename T>
void ThreadSafeVector<T>::push_back(const T& value)
{
	std::unique_lock<std::mutex> lock(the_mutex);

	the_vector.push_back(value);

	lock.unlock();
	the_condition_variable.notify_one();
}

template<typename T>
bool ThreadSafeVector<T>::try_pop_back(T& popped_value)
{
	std::lock_guard<std::mutex> lock(the_mutex);

	if (the_vector.empty())
	{
		return false;
	}

	popped_value = the_vector.back();
	the_vector.pop_back();

	return true;
}

template<typename T>
void ThreadSafeVector<T>::wait_and_pop_back(T& popped_value)
{
	std::unique_lock<std::mutex> lock(the_mutex);

	while (the_vector.empty())
	{
		the_condition_variable.wait(lock);
	}

	popped_value = the_vector.back();
	the_vector.pop_back();
}

template<typename T>
bool ThreadSafeVector<T>::try_at(std::size_t index, T& value) const
{
	std::lock_guard<std::mutex> lock(the_mutex);

	if (index >= the_vector.size())
	{
		return false;
	}

	value = the_vector[index];

	return true;
}

template<typename T>
void ThreadSafeVector<T>::clear()
{
	std::lock_guard<std::mutex> lock(the_mutex);

	the_vector.clear();
}
