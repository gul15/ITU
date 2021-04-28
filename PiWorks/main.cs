int var1 = Convert.ToInt32(Console.ReadLine());
int var2 = Convert.ToInt32(Console.ReadLine());
Console.WriteLine($"x = {var1}  y = {var2}");
var1 = var1 + var2;
var2 = var1 - var2;
var1 = var1 - var2;
Console.WriteLine($"x = {var1}  y = {var2}");
