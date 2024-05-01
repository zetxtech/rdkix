import pickle

from rdkix import Chem, RDConfig
from rdkix.Dbase import DbModule
from rdkix.Dbase.DbConnection import DbConnect

if RDConfig.usePgSQL:
  dbName = "::RDTests"
else:
  dbName = "data.sqlt"

molTblName = 'simple_mols1'
fpTblName = 'simple_mols1_fp'
conn = DbConnect(dbName, molTblName)
conn.AddTable(fpTblName, 'id varchar(10),autofragmentfp %s' % DbModule.binaryTypeName)
d = conn.GetData()
for smi, ID in d:
  print(repr(ID), repr(smi))
  mol = Chem.MolFromSmiles(smi)
  fp = Chem.RDKFingerprint(mol)
  pkl = pickle.dumps(fp)
  conn.InsertData(fpTblName, (ID, DbModule.binaryHolder(pkl)))
conn.Commit()
