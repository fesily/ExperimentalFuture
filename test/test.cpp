#include <experimental\future>

void then_test1()
{
	experimental::future<void> f;
	{
		std::promise<double> pm;
		f = experimental::make_then(pm).then([](std::future<double> ft)
		{
			auto t = ft.get();
			return (int)(t * t);
		}).then([](int f)
		{

		});
		pm.set_value(1234);
	}
	while (!f.is_ready())
	{
		std::this_thread::yield();
	}
	f.get();
}
void when_all_test1()
{
	experimental::promise<int> pm1;
	std::promise<int> pm2;
	experimental::promise<int> pm3;
	std::promise<int> pm4;

	experimental::future<int> f1 = pm1.get_future();
	std::future<int> f2 = pm2.get_future();
	experimental::shared_future<int> f3 = pm3.get_future();
	std::shared_future<int> f4 = pm4.get_future();
	auto when_all_ft = experimental::when_all(f1, f2, f3, f4);
	pm1.set_value(1);
	pm2.set_value(1);
	pm3.set_value(1);
	pm4.set_value(1);
	auto tupleFts = when_all_ft.get();
}
template<typename T>
void when_all_test2()
{
	std::vector<T> listPm(4);
	std::vector<decltype(std::declval<T>().get_future())> list(4);

	for (int i = 0; i < 4; ++i)
	{
		list[i] = listPm[i].get_future();
	}

	auto when_all_ft = experimental::when_all(list.begin(), list.end());

	for (int i = 0; i < 4; ++i)
	{
		listPm[i].set_value(1);
	}
	auto tupleFts = when_all_ft.get();
}
void when_any_test1()
{
	experimental::promise<int> pm1;
	std::promise<int> pm2;
	experimental::promise<int> pm3;
	std::promise<int> pm4;

	experimental::future<int> f1 = pm1.get_future();
	std::future<int> f2 = pm2.get_future();
	experimental::shared_future<int> f3 = pm3.get_future();
	std::shared_future<int> f4 = pm4.get_future();
	auto when_all_ft = experimental::when_any(f1, f2, f3, f4);
	pm3.set_value(1);
	pm1.set_value(2);
	pm2.set_value(3);
	pm4.set_value(4);
	auto ret = when_all_ft.get();
}
template<typename T>
void when_any_test2()
{
	std::vector<T> listPm(4);
	std::vector<decltype(std::declval<T>().get_future())> list(4);

	for (int i = 0; i < 4; ++i)
	{
		list[i] = listPm[i].get_future();
	}

	auto when_all_ft = experimental::when_any(list.begin(), list.end());

	for (int i = 0; i < 4; ++i)
	{
		listPm[i].set_value(1);
	}
	auto tupleFts = when_all_ft.get();
}
int main()
{
	experimental::promise<int> pm;
	std::future<int> f = pm.get_future();
	experimental::future<int> f1(std::move(f));
	f1.then([]()
	{

	});
	f1.on_error([](std::exception_ptr) {

	});
	std::future<int> f2(std::move(f1));
	int a = 999;
	while (a--)
	{
		when_all_test2<std::promise<int>>();
		when_all_test2<experimental::promise<int>>();
		when_all_test1();
		when_any_test1();
		when_any_test2<std::promise<int>>();
		when_any_test2<experimental::promise<int>>();
		then_test1();
	}
	using namespace  std::chrono_literals;
	std::this_thread::sleep_for(3s);
	return 0;
}

