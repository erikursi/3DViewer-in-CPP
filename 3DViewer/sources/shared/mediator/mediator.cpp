#include "mediator.h"

// Initialize the static members
std::unique_ptr<Mediator> Mediator::instance = nullptr;
std::once_flag Mediator::initInstanceFlag;