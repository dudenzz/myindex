import sqlite3
import re

sqlQuery = 'SELECT id, criteria from clinicaltrials'

conn = sqlite3.connect('trec2017.db')

with conn:
    cursor = conn.execute(sqlQuery)

    cur = conn.cursor()

    iter = 0

    incl = 'inclusion criteria'
    excl = 'exclusion criteria'

    for row in cursor:
        if iter%1000==0:
            print iter
        sqlArgs = []
        sqlArgs.append(row[0])
        inclusion = ''
        exclusion = ''
        id = row[0]
        text = row[1]
        text = text.lower()
        if (text == """
        please contact site for information.
      """ or text == """
        No eligibility criteria
      """):
            print 'No criteria given\n'
        else:
            text = text.split(incl)
            # print len(text)
            if (len(text) > 1):
                for i in range(1, len(text)):
                    inex = text[i].split(excl)
                    if len(inex) == 1:
                        inex = inex[0].split('  - ')
                        for j in range(1, len(inex)):
                            inside = inex[j].split(' no ')
                            if len(inside) == 1:
                                inside2 = inside[0].split(' not ')
                                if len(inside2) > 1:
                                    exclusion += inside2[1] + '\n'
                                else:
                                    inclusion += inside[0] + '\n'
                            else:
                                exclusion += inside[1] + '\n'
                    else:
                        for j in range(0, len(inex)):
                            if (len(text) <= 2):
                                if (j % 2 == 0):
                                    inclusion += inex[j] + '\n'
                                else:
                                    exclusion += inex[j] + '\n'
                            else:
                                if (j % 2 == 0):
                                    inclusion += inex[j] + '\n'
                                else:
                                    splt = re.split('\s\s+\d', inex[j])
                                    if len(splt) == 1:
                                        exclusion += inex[j] + '\n'
                                    else:
                                        for z in range(0, (len(splt) - 1)):
                                            exclusion += splt[z] + '\n'
            else:
                text = text[0].split(excl)
                if (len(text) > 1):
                    inclusion = text[0]
                    exclusion = text[1]
                else:
                    text = text[0].split(' no ')
                    if (len(text) > 1):
                        inclusion = text[0]
                        for i in range(1, len(text)):
                            exclusion += text[i] + '\n'
                    else:
                        text = text[0].split(' not ')
                        if (len(text) > 1):
                            inclusion = text[0]
                            for i in range(1, len(text)):
                                exclusion += text[i] + '\n'
                        else:
                            inclusion = text[0]


        if (inclusion!='' and exclusion!=''):
            hasclearinexclude=1
        else:
            hasclearinexclude = 0
        query = 'INSERT INTO inexclude (id,inclusion,exclusion,hasclearinexclude) VALUES(?,?,?,?)'
        cur.execute(query,(id,inclusion,exclusion,hasclearinexclude))
        #print 'ID = '+str(id)+'\n'
        #print 'INCLUSION:\n'
        #print  inclusion
        #print 'EXCLUSION:\n'
        #print exclusion
        iter +=1
