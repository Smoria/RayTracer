#pragma once

template<typename T, typename... U>
void UnusedVar(T&&, U&&...) {}