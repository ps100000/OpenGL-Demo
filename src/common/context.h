#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <functional>
#include <vector>

class context{
	public:
		context() = default;
		~context() = default;

		virtual void ctx_begin();
		virtual void ctx_end();

		void use(std::function<void()> lambda);

		static void use(std::vector<std::reference_wrapper<context>> ctxs, std::function<void()> lambda);
};

class generic_context: public context{
	private:
		void ctx_begin() override;
		void ctx_end() override;

		std::function<void()> begin_func;
		std::function<void()> end_func;
	public:
		generic_context(
				std::function<void()> begin = [](){},
				std::function<void()> end = [](){});

		void set_func(
				std::function<void()> begin,
				std::function<void()> end);
};

#endif
