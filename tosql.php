<?php

$link = msqli_connect("localhost", "kdadev", "password", "kdadevflowers");

if($link === false || !isset($_POST) {
	echo "<script> alert('Ошибка. Данные не загружены.'); window.location.assign('webpage.html');</script>";
}
else {
	mysqli_set_charset($link, "utf8");
	$name = mysqli_real_escape_string($link, $_POST['name']);
	$surname = mysqli_real_escape_string($link, $_POST['surname']);
	$phone = mysqli_real_escape_string($link, $_POST['phone']);
	$email = mysqli_real_escape_string($link, $_POST['email']);
	
	$sql = "INSERT INTO contact SET name = " . $name . ", surname = " . $surname . ", phone = " . $phone . ", email = " . $email;
	$result = msqli_query($link, $sql);
	if($result === false) {
		echo "<script> alert('Ошибка. Данные не загружены.'); window.location.assign('webpage.html');</script>";
	}
	else {
		echo "<script> alert('Данные успешно загружены.'); window.location.assign('webpage.html');</script>";
	}
}
   ?>
