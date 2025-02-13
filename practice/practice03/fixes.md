# Bugs and Fixes

## sumRange
- **Bug**: The loop condition `i < end` excluded the `end` value.
- **Fix**: Changed the condition to `i <= end` to include the `end` value.

## containsNegative
- **Bug**: The loop condition `i <= numbers.size()` caused out-of-bounds access. The condition `if (numbers[i] > 0)` was incorrect.
- **Fix**: Changed the loop condition to `i < numbers.size()`. Fixed the condition to `if (numbers[i] < 0)`.

## findMax
- **Bug**: The loop condition `i <= numbers.size()` caused out-of-bounds access. Returning `0` for an empty vector was not ideal.
- **Fix**: Changed the loop condition to `i < numbers.size()`. Threw an exception for an empty vector.