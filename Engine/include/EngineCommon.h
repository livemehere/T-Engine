#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>


#define LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
#define LOG_INFO(...)     spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)