#pragma once


// Singleton pattern template
template<typename T>
class singleton
{
public:
	
	static T& instance();

	// Avoid copy constructor, = copy operator
	singleton(const singleton&) = delete;
	singleton& operator= (const singleton) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;

protected:

	// Avoid object declaration
	singleton() = default;
	virtual ~singleton() = default;

	// Use to access member variables
	struct token {};
};

/// <summary>
/// Get the only existing instance of the current class
/// </summary>
/// <typeparam name="T"> Type of class </typeparam>
/// <returns></returns>
template<typename T>
T& singleton<T>::instance()
{
	static T instance{ token{} };
	return instance;
}

