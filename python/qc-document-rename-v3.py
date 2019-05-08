# -*- coding: utf-8 -*- 
import  sys
import xlrd
import os.path
import re

def open_excel(file= 'file.xls'):
    try:
        data = xlrd.open_workbook(file)
        return data
    except Exception as e:
        print (str(e))

def excel_table_byindex(file= 'file.xls',colnameindex=0,by_index=0):
    data = open_excel(file)
    table = data.sheets()[by_index]
    nrows = table.nrows 
    ncols = table.ncols 
    colnames =  table.row_values(colnameindex) 
    list =[]
    for rownum in range(1,nrows):

         row = table.row_values(rownum)
         if row:
             app = {}
             for i in range(len(colnames)):
                app[colnames[i]] = row[i] 
             list.append(app)
    return list

def excel_table_byname(file= 'file.xls',colnameindex=0,by_name=u'Sheet1'):
    data = open_excel(file)
    table = data.sheet_by_name(by_name)
    nrows = table.nrows 
    colnames =  table.row_values(colnameindex) 
    list =[]
    for rownum in range(1,nrows):
         row = table.row_values(rownum)
         if row:
             app = {}
             for i in range(len(colnames)):
                app[colnames[i]] = row[i]
             list.append(app)
    return list
    
def replace_invalid_filename_char(filename, replaced_char='-'):
    '''Replace the invalid characaters in the filename with specified characater.
    The default replaced characater is '_'.
    e.g. 
    C/C++ -> C_C++
    '''
    valid_filename = filename
    invalid_characaters = '\\/:*?"<>|'
    for c in invalid_characaters:
        #print 'c:', c
        valid_filename = valid_filename.replace(c, replaced_char)

    return valid_filename 


def main():
    
    if len(sys.argv) < 3:
        print('cmd python qc-ducomment-rename.py document-folder SolutionTitles.xls')
        sys.exit()
    tables = excel_table_byname(sys.argv[2],0,"SolutionTitles")     
    for parent,dirnames,filenames in os.walk(sys.argv[1]):
        for filename in filenames:                      
            name, ext = os.path.splitext(filename)
            aliasname = filename.replace('_','-').upper()
            #aliasname = aliasname.upper()
            aliasname = replace_invalid_filename_char(aliasname)
            print("the full name of the file is: " + aliasname)
            for row in tables:              
              if aliasname.find(row['DCN']+'-') == 0:
                if re.match(row['DCN']+'-'+'[A-Z]'+'-',aliasname):
                  finalname = re.match(row['DCN']+'-'+'[A-Z]'+'-',aliasname).group()+re.sub('[^A-Za-z0-9]', '-', row['Title'].encode('utf-8').decode())
                else:
                  finalname = row['DCN']+"-"+re.sub("[^A-Za-z0-9]", "-", row['Title'].encode('utf-8').decode())
                finalname = finalname.replace('--','-')
                finalname = finalname.replace('--','-')
                finalname = finalname.replace('--','-')
                full_finalname = os.path.join(parent,finalname)+ext           
                if os.path.join(parent,filename) != full_finalname:
                    print (os.path.join(parent,filename)+ "->" + os.path.join(parent,finalname)+ext)
                    os.rename(os.path.join(parent,filename), os.path.join(parent,finalname)+ext)
                    break
     

if __name__=="__main__":
    main()