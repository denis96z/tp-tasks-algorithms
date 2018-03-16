#ifndef BAD_INPUT_H
#define BAD_INPUT_H

#include <exception>

class bad_input : public std::exception {
	public:
		bad_input(const char *msg) noexcept : msg(msg) {
			//NOP
		}

		~bad_input() noexcept override = default;

		const char* what() const noexcept override {
			return msg;
		}

	private:
		const char *msg = nullptr;
};

#endif //BAD_INPUT_H
