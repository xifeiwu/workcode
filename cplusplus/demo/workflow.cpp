#include <iostream>
int while_flow()
{
    int sum = 0, val  = 1;
    // keep executing the while as long as val is less than or equal t
    while (val <= 10)  {
        sum += val;   // assigns sum + val to sum
        ++val;        // add 1 to val
    }
    std::cout << "Sum of 1 to 10 inclusive is "
              << sum << std::endl;
    return 0;
}

int for_flow()
{
    int val, count = 0;
    int sum = 0;
    // sum values from 1 through 10 inclusive
    for (val = 1; val <= 10; ++val, count++, std::cout << count) {
        sum += val;  // equivalent to sum = sum + val
//        std::cout << count;
    }
    std::cout << std::endl;
    std::cout << "Sum of 1 to 10 inclusive is "
              << sum << std::endl;
}

int unknown_input()
{
    int value = 0, sum = 0;
    while (std::cin >> value)
        sum += value; // equivalent to sum = sum + value
    std::cout << "Sum is: " << sum << std::endl;
    return 0;
}
int main()
{
//    while_flow();
//    for_flow();
    unknown_input();
    return 0;
}
