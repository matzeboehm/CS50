-- Keep a log of any SQL queries you execute as you solve the mystery.
.schema

-- get description of what exactly happened
-- Crime happened at 10:15 am
SELECT description FROM crime_scene_reports WHERE (day = 28 AND month = 7 AND year = 2020 AND street = "Chamberlin Street");

-- thief drove away from courthouse parking up to 10 minutes after theft, so from 10:15 to 10:25 --> security courthouse
-- before theft, thief took money from ATM at Fifer street
-- thief wants to take earliest flight next day, person on other side of telephone books ticket
SELECT name, transcript FROM interviews WHERE (day = 28 AND month = 7 AND year = 2020) AND transcript LIKE "%courthouse%";

-- which license plates exited the courthouse on this day in the 10 minute range
SELECT hour, minute, license_plate, activity FROM courthouse_security_logs WHERE (day = 28 AND month = 7 AND year = 2020 AND hour = 10 and minute>=15 AND minute <=25);

-- who are those people who left at that time
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE (day = 28 AND month = 7 AND year = 2020 AND hour = 10 and minute>=15 AND minute <=25)) ORDER BY name;
--> Suspects: Amber, Danielle, Elizabeth, Ernest, Evelyn, Patrick, Russell


--> ATM before courthouse, so everything before 10:15
-- person who owns a license plate that was on courthouse this day
-- person who got money on this day in Fifer Street
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE (day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street"))
    AND license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE (day = 28 AND month = 7 AND year = 2020 AND hour = 10 and minute>=15 AND minute <=25))
    ORDER BY name;
-- Result: Danielle, Elizabeth, Ernest, Russell

-- Who had a call in the time frame when they left the building, so between 10:15 and 10:25, duration less than a minute
SELECT name from people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE (day = 28 AND month = 7 AND year = 2020 AND duration <= 60)) ORDER BY name;
-- This leads to:
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE (day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street"))
    AND license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE (day = 28 AND month = 7 AND year = 2020 AND hour = 10 and minute>=15 AND minute <=25))
    AND phone_number IN (SELECT caller FROM phone_calls WHERE (day = 28 AND month = 7 AND year = 2020 AND duration <= 60))
    ORDER BY name;
-- Suspects: Ernest, Russel


-- person who is also a passanger on the earliest flight the next day from Fiftyville
-- ID of Fiftyville airport:
SELECT id FROM airports WHERE city LIKE "Fiftyville";
-- Airport ID is 8

-- flight ID from earlierst flight from origin airport 8
SELECT id, MIN(hour), minute FROM flights WHERE (origin_airport_id = 8 AND day = 29 AND month = 7 AND year = 2020);
-- flight ID is 36

-- Desitnation of flight: Boston
SELECT city FROM airports
JOIN flights ON  airports.id = flights.destination_airport_id
WHERE flights.id = 36;

-- Who is on flight 36 and fullfils all other requirements
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE (day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street"))
    AND license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE (day = 28 AND month = 7 AND year = 2020 AND hour = 10 and minute>=15 AND minute <=25))
    AND phone_number IN (SELECT caller FROM phone_calls WHERE (day = 28 AND month = 7 AND year = 2020 AND duration <= 60))
    AND people.passport_number IN (SELECT passengers.passport_number FROM passengers WHERE (passengers.flight_id = 36))
    ORDER BY name;
-- Suspect: Ernest

-- Who did he call
SELECT name FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE (day = 28 AND month = 7 AND year = 2020 AND duration <= 60
    AND caller = (SELECT phone_number FROM people WHERE name = "Ernest")));
-- He called Berthold