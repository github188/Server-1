#pragma once
#include <functional>
namespace WwFoundation
{
	class RAII
	{
		typedef std::function<void()> ExitClean;
	private:
		ExitClean onExitScope_;
		bool dismissed_;
	public:
		explicit RAII(const ExitClean& onExitScope = []() {}) /*noexcept*/
			: onExitScope_(onExitScope), dismissed_(false) { }

		~RAII() /*noexcept*/
		{
			if (!dismissed_)
				onExitScope_();
		}

		RAII(RAII&& raii)/* noexcept */
			:onExitScope_(raii.onExitScope_), dismissed_(raii.dismissed_)
		{
			raii.dismissed_ = true;
		}

		RAII& operator=(RAII&& raii) /*noexcept*/
		{
			onExitScope_ = raii.onExitScope_;
			dismissed_ = raii.dismissed_;
			raii.onExitScope_ = nullptr;
			raii.dismissed_ = true;
			return *this;
		}

		void Dismiss() /*noexcept*/
		{
			dismissed_ = true;
		}
	private:
		RAII(RAII const&) /*= delete*/;
		RAII& operator=(const RAII&)/* = delete*/;
	};
}
	