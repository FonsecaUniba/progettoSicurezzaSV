CREATE DATABASE `signaturesvm`;

USE 'signaturesvm';

CREATE TABLE `svmodels` (
  `UserID` int(11) NOT NULL AUTO_INCREMENT,
  `ModelPATH` varchar(260) NOT NULL,
  `Threshold` decimal(5,2) NOT NULL DEFAULT '0.70',
  PRIMARY KEY (`UserID`)
);