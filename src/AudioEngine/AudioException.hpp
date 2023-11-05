// Copyright 2023 Clément Chambard
#ifndef AUDIOEXCEPTION_H_
#define AUDIOEXCEPTION_H_

#include <exception>
#include <string>

class AudioException : public std::exception {
public:
  explicit AudioException(std::string const &whatString)
      : m_what("Audio exception: " + whatString) {}
  ~AudioException() = default;

  const char *what() const noexcept override;

private:
  std::string m_what;
};

#endif // AUDIOEXCEPTION_H_
