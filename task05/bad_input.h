#ifndef BAD_INPUT_H
#define BAD_INPUT_H

#include <exception>

class bad_input : public std::exception {
public:
    bad_input(const char *msg) _GLIBCXX_USE_NOEXCEPT : msg(msg) {
        //NOP
    }

    virtual ~bad_input() _GLIBCXX_USE_NOEXCEPT = default;

    const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return msg;
    }

private:
    const char *msg = nullptr;
};

#endif //BAD_INPUT_H
