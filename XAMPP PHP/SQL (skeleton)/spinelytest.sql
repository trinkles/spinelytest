-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Mar 04, 2024 at 01:23 PM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `spinelytest`
--

-- --------------------------------------------------------

--
-- Table structure for table `device_calibration`
--

CREATE TABLE `device_calibration` (
  `calibrationId` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `cervical_max_angle` double NOT NULL,
  `cervical_min_angle` double NOT NULL,
  `cervical_avg_angle` double NOT NULL,
  `thoracic_max_angle` double NOT NULL,
  `thoracic_min_angle` double NOT NULL,
  `thoracic_avg_angle` double NOT NULL,
  `lumbar_max_angle` double NOT NULL,
  `lumbar_min_angle` double NOT NULL,
  `lumbar_avg_angle` double NOT NULL,
  `left_midAxLine_max_angle` double NOT NULL,
  `left_midAxLine_min_angle` double NOT NULL,
  `left_midAxLine_avg_angle` double NOT NULL,
  `right_midAxLine_max_angle` double NOT NULL,
  `right_midAxLine_min_angle` double NOT NULL,
  `right_midAxLine_avg_angle` double NOT NULL,
  `calibration_timestamp` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `monitoring`
--

CREATE TABLE `monitoring` (
  `monitoringId` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `sessionId` int(11) NOT NULL,
  `cervical` double NOT NULL,
  `thoracic` double NOT NULL,
  `lumbar` double NOT NULL,
  `leftMidAx` double NOT NULL,
  `rightMidAx` double NOT NULL,
  `postureStatus` text NOT NULL,
  `monitoring_timestamp` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `progress_report`
--

CREATE TABLE `progress_report` (
  `progressId` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `sessionId` int(11) NOT NULL,
  `results_proper` double NOT NULL,
  `progress_proper` double NOT NULL,
  `prev_results` double NOT NULL,
  `current_results` double NOT NULL,
  `final_results` double NOT NULL,
  `progressReport_date` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `sessions`
--

CREATE TABLE `sessions` (
  `sessionId` int(11) NOT NULL,
  `userId` int(11) NOT NULL,
  `percent_proper` double DEFAULT NULL,
  `date_start` datetime NOT NULL DEFAULT current_timestamp(),
  `date_end` datetime DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `userId` int(11) NOT NULL,
  `name` text NOT NULL,
  `username` text NOT NULL,
  `email` text NOT NULL,
  `password` text NOT NULL,
  `signUp_date` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Triggers `users`
--
DELIMITER $$
CREATE TRIGGER `prevent_update_signUp_date` BEFORE UPDATE ON `users` FOR EACH ROW BEGIN
    IF NEW.signUp_date <> OLD.signUp_date THEN
        SET NEW.signUp_date = OLD.signUp_date;
    END IF;
END
$$
DELIMITER ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `device_calibration`
--
ALTER TABLE `device_calibration`
  ADD PRIMARY KEY (`calibrationId`),
  ADD KEY `fk_calibration_userId` (`userId`);

--
-- Indexes for table `monitoring`
--
ALTER TABLE `monitoring`
  ADD PRIMARY KEY (`monitoringId`),
  ADD KEY `fk_monitoring_userId` (`userId`),
  ADD KEY `fk_monitoring_sessionId` (`sessionId`);

--
-- Indexes for table `progress_report`
--
ALTER TABLE `progress_report`
  ADD PRIMARY KEY (`progressId`),
  ADD KEY `fk_progress_userId` (`userId`),
  ADD KEY `fk_progress_sessionId` (`sessionId`);

--
-- Indexes for table `sessions`
--
ALTER TABLE `sessions`
  ADD PRIMARY KEY (`sessionId`),
  ADD KEY `fk_sessions_userId` (`userId`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`userId`),
  ADD UNIQUE KEY `username` (`username`) USING HASH,
  ADD UNIQUE KEY `email` (`email`) USING HASH;

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `device_calibration`
--
ALTER TABLE `device_calibration`
  MODIFY `calibrationId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=201;

--
-- AUTO_INCREMENT for table `monitoring`
--
ALTER TABLE `monitoring`
  MODIFY `monitoringId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=401;

--
-- AUTO_INCREMENT for table `progress_report`
--
ALTER TABLE `progress_report`
  MODIFY `progressId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=501;

--
-- AUTO_INCREMENT for table `sessions`
--
ALTER TABLE `sessions`
  MODIFY `sessionId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=301;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `userId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=101;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `device_calibration`
--
ALTER TABLE `device_calibration`
  ADD CONSTRAINT `fk_calibration_userId` FOREIGN KEY (`userId`) REFERENCES `users` (`userId`);

--
-- Constraints for table `monitoring`
--
ALTER TABLE `monitoring`
  ADD CONSTRAINT `fk_monitoring_sessionId` FOREIGN KEY (`sessionId`) REFERENCES `sessions` (`sessionId`),
  ADD CONSTRAINT `fk_monitoring_userId` FOREIGN KEY (`userId`) REFERENCES `users` (`userId`);

--
-- Constraints for table `progress_report`
--
ALTER TABLE `progress_report`
  ADD CONSTRAINT `fk_progress_sessionId` FOREIGN KEY (`sessionId`) REFERENCES `sessions` (`sessionId`),
  ADD CONSTRAINT `fk_progress_userId` FOREIGN KEY (`userId`) REFERENCES `users` (`userId`);

--
-- Constraints for table `sessions`
--
ALTER TABLE `sessions`
  ADD CONSTRAINT `fk_sessions_userId` FOREIGN KEY (`userId`) REFERENCES `users` (`userId`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
