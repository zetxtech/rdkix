CREATE INDEX fpidx ON pgbfp USING gin (f);
CREATE INDEX maccsfpidx ON pgbfp USING gin (maccsf);

SET rdkix.tanimoto_threshold = 0.5;
SET rdkix.dice_threshold = 0.6;


SET enable_indexscan=off;
SET enable_bitmapscan=off;
SET enable_seqscan=on;

SELECT
    id, tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
	pgbfp
WHERE rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % f
ORDER BY sml DESC, id limit 10;

SELECT
    id, dice_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
	pgbfp
WHERE rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) # f
ORDER BY sml DESC, id limit 10;

SELECT
    id, tanimoto_sml(maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), maccsf) AS sml
FROM
	pgbfp
WHERE maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % maccsf
ORDER BY sml DESC, id limit 10;


SET enable_indexscan=on;
SET enable_bitmapscan=on;
SET enable_seqscan=off;

SELECT
    id, tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
	pgbfp
WHERE rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % f
ORDER BY sml DESC, id limit 10;

SELECT
    id, dice_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
	pgbfp
WHERE rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) # f
ORDER BY sml DESC, id limit 10;

SELECT
    id, tanimoto_sml(maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), maccsf) AS sml
FROM
	pgbfp
WHERE maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % maccsf
ORDER BY sml DESC, id limit 10;

SET enable_indexscan=on;
SET enable_bitmapscan=on;
SET enable_seqscan=on;

SELECT
    id, tanimoto_sml(maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), maccsf) AS sml
FROM
	pgbfp
WHERE maccs_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % maccsf
ORDER BY sml DESC, id limit 10;

DROP INDEX fpidx;
DROP INDEX maccsfpidx;
