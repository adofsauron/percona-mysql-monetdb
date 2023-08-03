--!/usr/bin/env bash

-- This Source Code Form is subject to the terms of the Mozilla Public
-- License, v. 2.0.  If a copy of the MPL was not distributed with this
-- file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- Copyright 2017-2018 MonetDB Solutions B.V.

-- Sccsid:     @(#)dss.ri	2.1.8.1
-- TPCD Benchmark Version 8.0

START TRANSACTION;

--ALTER TABLE region DROP PRIMARY KEY;
--ALTER TABLE nation DROP PRIMARY KEY;
--ALTER TABLE part DROP PRIMARY KEY;
--ALTER TABLE supplier DROP PRIMARY KEY;
--ALTER TABLE partsupp DROP PRIMARY KEY;
--ALTER TABLE orders DROP PRIMARY KEY;
--ALTER TABLE lineitem DROP PRIMARY KEY;
--ALTER TABLE customer DROP PRIMARY KEY;


-- For table region
ALTER TABLE region SET READ WRITE;

-- For table nation
ALTER TABLE nation SET READ WRITE;

-- For table part
ALTER TABLE part SET READ WRITE;

-- For table supplier
ALTER TABLE supplier SET READ WRITE;

-- For table partsupp
ALTER TABLE partsupp SET READ WRITE;

-- For table customer
ALTER TABLE customer SET READ WRITE;

-- For table orders
ALTER TABLE orders SET READ WRITE;

-- For table lineitem
ALTER TABLE lineitem SET READ WRITE;

COMMIT;
