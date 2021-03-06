//协程的定时器
//如果定时器被取消了，则会抛 timer_canceled_exception 异常
//不使用co_await而单独sleep_for/sleep_until，是错误的用法，并不能达到等待的目的。而仅仅是添加了一个无效的定时器，造成无必要的内存使用
//
#pragma once

namespace resumef
{
	struct scheduler;

	RF_API future_vt sleep_until_(const std::chrono::system_clock::time_point& tp_, scheduler & scheduler_);

	inline future_vt sleep_for_(const std::chrono::system_clock::duration& dt_, scheduler & scheduler_)
	{
		return std::move(sleep_until_(std::chrono::system_clock::now() + dt_, scheduler_));
	}

	template<class _Rep, class _Period>
	inline future_vt sleep_for(const std::chrono::duration<_Rep, _Period>& dt_, scheduler & scheduler_)
	{
		return sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), scheduler_);
	}
	template<class _Rep, class _Period>
	inline future_vt sleep_for(const std::chrono::duration<_Rep, _Period>& dt_)
	{
		return sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), *this_scheduler());
	}

	template<class _Clock, class _Duration = typename _Clock::duration>
	inline future_vt sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_, scheduler & scheduler_)
	{
		return sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), scheduler_);
	}
	template<class _Clock, class _Duration>
	inline future_vt sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_)
	{
		return sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), *this_scheduler());
	}

	template <class Rep, class Period>
	inline future_vt operator co_await(std::chrono::duration<Rep, Period> dt_)
	{
		return sleep_for(dt_);
	}

}