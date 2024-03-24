# put the file in dataset/sf1, same folder with snapshot and incremental
import datetime
import pandas as pd

filename = ["Account", "AccountTransferAccount", "AccountWithdrawAccount", "AccountRepayLoan", "LoanDepositAccount", "MediumSignInAccount", "PersonInvestCompany", "CompanyInvestCompany", "PersonApplyLoan", "CompanyApplyLoan", "PersonGuaranteePerson", "CompanyGuaranteeCompany", "PersonOwnAccount", "CompanyOwnAccount"]
coloum_name = "createTime"

filename = ["CompanyGuaranteeCompany", "PersonOwnAccount", "CompanyOwnAccount"]

def str_to_int13(t):
    int_t13 = datetime.datetime.strptime(str(t),"%Y-%m-%d %H:%M:%S.%f")
    return int(int_t13.timestamp() * 1000)

for i in range(len(filename)):
    print('Now in file: ' + filename[i])
    df = pd.read_csv('snapshot/' + filename[i] + '.csv', sep='|')
    df[coloum_name] = pd.to_datetime(df[coloum_name], format='ISO8601')
    df.to_csv('snapshot/' + filename[i] + '.csv', index = False, sep = '|')

    df = pd.read_csv('snapshot/' + filename[i] + '.csv', sep='|')
    df[coloum_name] = df[coloum_name].map(str_to_int13)
    df.to_csv('snapshot/' + filename[i] + '.csv', index = False, sep = '|')