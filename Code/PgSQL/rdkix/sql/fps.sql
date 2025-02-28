SET extra_float_digits=0;
SELECT dice_sml(rdkix_fp('c1ccccc1'::mol),rdkix_fp('c1ccncc1'::mol));
SELECT tversky_sml(rdkix_fp('c1ccccc1'::mol),rdkix_fp('c1ccncc1'::mol),0.5,0.5);
SELECT tanimoto_sml(rdkix_fp('c1ccccc1'::mol),rdkix_fp('c1ccncc1'::mol));
SELECT tversky_sml(rdkix_fp('c1ccccc1'::mol),rdkix_fp('c1ccncc1'::mol),1.0,1.0);
SELECT dice_sml(atompair_fp('c1ccccc1'::mol),atompair_fp('c1ccncc1'::mol));
SELECT dice_sml(torsion_fp('c1ccccc1'::mol),torsion_fp('c1ccncc1'::mol));
SELECT dice_sml(morgan_fp('c1ccccc1'::mol,2),morgan_fp('c1ccncc1'::mol,2));
SELECT dice_sml(morgan_fp('c1ccccc1'::mol),morgan_fp('c1ccncc1'::mol));
SELECT dice_sml(morganbv_fp('c1ccccc1'::mol,2),morganbv_fp('c1ccncc1'::mol,2));
SELECT dice_sml(morganbv_fp('c1ccccc1'::mol),morganbv_fp('c1ccncc1'::mol));
SELECT dice_sml(featmorgan_fp('c1ccccc1'::mol,2),featmorgan_fp('c1ccncc1'::mol,2));
SELECT dice_sml(featmorgan_fp('c1ccccc1'::mol),featmorgan_fp('c1ccncc1'::mol));
SELECT dice_sml(featmorganbv_fp('c1ccccc1'::mol,2),featmorganbv_fp('c1ccncc1'::mol,2));
SELECT dice_sml(featmorganbv_fp('c1ccccc1'::mol),featmorganbv_fp('c1ccncc1'::mol));
SELECT tanimoto_sml(maccs_fp('c1ccccc1'::mol),maccs_fp('c1ccncc1'::mol));
SELECT dice_sml(maccs_fp('c1ccccc1'::mol),maccs_fp('c1ccncc1'::mol));

SET rdkix.tanimoto_threshold = 0.4;
SELECT
    id, tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
pgbfp
WHERE rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) % f
ORDER BY sml DESC, id LIMIT 10;

SET rdkix.tanimoto_threshold = 0.5;
SET rdkix.dice_threshold = 0.5;

SELECT
    id, dice_sml(torsion_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
pgtorsfp
WHERE torsion_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) # f
ORDER BY sml DESC, id;

SELECT
    id, dice_sml(atompair_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), f) AS sml
FROM
pgpairfp
WHERE atompair_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol) # f
ORDER BY sml DESC, id LIMIT 10;

select tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), rdkix_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.rdkix_fp_size = 512;
select tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), rdkix_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.rdkix_fp_size = 1024;
select tanimoto_sml(rdkix_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), rdkix_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

select tanimoto_sml(layered_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), layered_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.layered_fp_size = 512;
select tanimoto_sml(layered_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), layered_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.layered_fp_size = 1024;
select tanimoto_sml(layered_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), layered_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

select tanimoto_sml(morganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), morganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.morgan_fp_size = 512;
select tanimoto_sml(morganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), morganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.morgan_fp_size = 1024;
select tanimoto_sml(morganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), morganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

select tanimoto_sml(featmorganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), featmorganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.featmorgan_fp_size = 512;
select tanimoto_sml(featmorganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), featmorganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.featmorgan_fp_size = 1024;
select tanimoto_sml(featmorganbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), featmorganbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

select tanimoto_sml(torsionbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), torsionbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.hashed_torsion_fp_size = 512;
select tanimoto_sml(torsionbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), torsionbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.hashed_torsion_fp_size = 1024;
select tanimoto_sml(torsionbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), torsionbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

select tanimoto_sml(atompairbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), atompairbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.hashed_atompair_fp_size = 512;
select tanimoto_sml(atompairbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), atompairbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;
set rdkix.hashed_atompair_fp_size = 1024;
select tanimoto_sml(atompairbv_fp('O=C1CC(OC2=CC=CC=C12)C1=CC=CC=C1'::mol), atompairbv_fp('N=C1CC(NC2=CC=CC=C12)C1=CC=CC=C1'::mol)) AS sml;

