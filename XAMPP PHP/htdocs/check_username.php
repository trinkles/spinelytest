<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve username from the POST request
    $user_username = $_POST["username"]; // Use a different variable name here

    // Create a connection to the MySQL database
    $servername = "localhost"; // Change this to your MySQL server hostname
    $username_db = "root"; // Change this to your MySQL username
    $password_db = ""; // Change this to your MySQL password
    $dbname = "spinelytest"; // Change this to your MySQL database name

    $conn = new mysqli($servername, $username_db, $password_db, $dbname);

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Check if the username already exists
    $sql_check_username = "SELECT * FROM users WHERE username = '$user_username'"; // Use the correct variable name here
    $result_username = $conn->query($sql_check_username);

    if ($result_username->num_rows > 0) {
        echo "Username already exists";
    } else {
        echo "Username available";
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
