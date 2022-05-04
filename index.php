<?php

/**
 * Функция проверки числа на чётность
 * Применение:
 * isEven(2) -> True
 * isEven(3) -> False
 * @param int $value
 * @return bool
 */
function isEven(int $value)
{
    return $value % 2 == 0;
}

/**
 * Решает следующую задачу:
 * «N школьников делят k яблок поровну так, чтобы каждому достались только целые яблоки,
 * остальные яблоки остаются в корзинке. Определить, сколько яблок достанется каждому школьнику и
 * сколько яблок останется в корзинке».
 * Пример использования:
 * appleDivision(6, 50) -> [8, 2]
 * appleDivision(1, 10) -> [10, 0]
 * @param int $countStudents - кол-во школьников
 * @param int $countApples - кол-во яблок
 * @return array
 */
function appleDivision(int $countStudents, int $countApples)
{
    // Измените код функции
    $studentApples = intdiv($countApples, $countStudents);  // Кол-во яблок для каждого школьника
    $applesLeft = $countApples % $countStudents;     // Кол-во яблок, оставшиеся в корзинке
    return [$studentApples, $applesLeft];
}

/**
 * Функция решения квадратного уравнения a*x^2+b*x+c
 * Учитываются только вещественные корни (без комплексных)
 * Порядок корней в ответе не важен
 * Пример использования:
 * solveQuadraticEquation(2, 4, 2) -> [-1.0] (1 корень)
 * solveQuadraticEquation(3.2, -7.8, 1) -> [2.30, 0.14] (2 корня)
 * solveQuadraticEquation(8, 4, 2) -> [] (вещественных корней нет)
 * @param float $a
 * @param float $b
 * @param float $c
 * @return array - массив корней квадратного уравнения
 */
function solveQuadraticEquation(float $a, float $b, float $c)
{
    $D = $b * $b - 4 * $a * $c;
	
	if ($D < 0) {
		return [];
	}
	elseif ($D == 0) {
		return [(-$b) / (2 * $a)];
	}
	elseif($D > 0) {
		return [(-$b + sqrt($D)) / (2 * $a), (-$b - sqrt($D)) / (2 * $a)];
	}
}


/**
 * Находит сумму всех чисел числа
 * Пример использования:
 * digitsSum(1234) -> 10
 * digitsSum(5566) -> 22
 * digitsSum(123456789) -> 45
 * @param int $number - исходное число
 * @return int - сумма всех чисел числа
 */
function digitsSum(int $number)
{
	$number = abs($number);
	$answer = 0;
    while($number > 0) {
		$answer += $number % 10;
		$number = intdiv($number, 10);
	}
    return $answer;
}


// Секция тестирования
// Расскоментируйте код для проверки своих функций

//var_dump(isEven(2)); // True
//var_dump(appleDivision(6, 50));   // [8, 2]
//var_dump(solveQuadraticEquation(2, 4, 2));     // [-1.0]
//var_dump(digitsSum(1234));     // 10
