import sqlite3
import xml.etree.ElementTree as ET
import os


def getElementValue(element,tag):
    try:
        return element.find(tag).text
    except:
        return ''
def getElementsValue(element,tag):
    text = ''
    try:
        elements = element.findall(tag)
        for e in elements:
            text += e.text+'\n'
        return text
    except:
        return ''

def getElementTag(root,tag):
    try:
        return root.find(tag)
    except:
        return None

def addSqlArg(element,tag):
    sqlElements.append(getElementValue(element,tag))

def addSqlArgs(element,tag):
    sqlElements.append(getElementsValue(element, tag))

def addSqlNumericArg(element,tag):
    text = getElementValue(element,tag)
    text = text.split()
    try:
        value = int(text[0])
        sqlElements.append(value)
    except:
        sqlElements.append(-1)
def addComplexSqlArg(element,tags):
    text = ''
    for tag in tags:
        t = getElementValue(element,tag)
        if (t != ''):
            text += t+'\n'
    sqlElements.append(text)

def addSqlArgsForLoop(root,element,tags):
    text = ''
    elements = root.findall(element)
    for e in elements:
        for t in tags:
            text += getElementValue(e,t)
            text += '\n'
    sqlElements.append(text)

def insertIntoDatabase(tableName, argNumber, args):
    sqlQuery = 'INSERT INTO ' + tableName +'(nct_id, brief_title, official_title, brief_summary, detailed_description, overall_status, start_date, primary_completion_date, completion_date, ' \
                                           'phase, study_type, study_design_info, primary_outcome, secondary_outcome, condition, arm_group, intervention, criteria, gender, minimum_age, ' \
                                           'maximum_age, healthy_volunteers, condition_browse, intervention_browse, keyword) VALUES('
    for i in range(0,argNumber):
        sqlQuery+='?'
        if (i!=argNumber-1):
            sqlQuery+=','
    sqlQuery+=')'
    #print sqlQuery
    cur.execute(sqlQuery,args)

conn = sqlite3.connect('trec2017.db')
rootdir = 'M:\\trec\\2017\\abstracts\\clinicaltrials_xml.tar\\clinicaltrials_xml'
#rootdir = 'C:\\trec\\2017\\abstracts\\clinicaltrials_xml\\clinicaltrials_xml'

with conn:
    cur = conn.cursor()
    iter = 0
    for subdir, dirs, files in os.walk(rootdir):
        for file in files:
            if (iter % 1000 == 0):
                print iter

            sqlElements = []

            tree = ET.parse(os.path.join(subdir, file))
            root = tree.getroot()

            elem = getElementTag(root, 'id_info')
            addSqlArg(elem,'nct_id')
            addSqlArg(root, 'brief_title')
            addSqlArg(root, 'official_title')
            elem = getElementTag(root, 'brief_summary')
            addSqlArg(elem, 'textblock')
            elem = getElementTag(root,'detailed_description')
            addSqlArg(elem, 'textblock')
            addSqlArg(root, 'overall_status')
            addSqlArg(root, 'start_date')
            addSqlArg(root, 'primary_completion_date')
            addSqlArg(root, 'completion_date')
            addSqlArg(root, 'phase')
            addSqlArg(root, 'study_type')
            elem = getElementTag(root,'study_design_info')
            addComplexSqlArg(elem,['intervention_model','primary_purpose','masking'])
            addSqlArgsForLoop(root,'primary_outcome',['measure','description'])
            addSqlArgsForLoop(root, 'secondary_outcome', ['measure', 'description'])
            addSqlArg(root,'condition')
            addSqlArgsForLoop(root, 'arm_group', ['arm_group_label', 'arm_group_type','description'])
            addSqlArgsForLoop(root, 'intervention', ['intervention_type', 'intervention_name', 'description'])
            elem = getElementTag(root, 'eligibility')
            el2 = getElementTag(elem,'criteria')
            addSqlArg(el2, 'textblock')
            addSqlArg(elem,'gender')
            addSqlNumericArg(elem, 'minimum_age')
            addSqlNumericArg(elem,'maximum_age')
            addSqlArg(elem, 'healthy_volunteers')
            elem = getElementTag(root,'condition_browse')
            addSqlArgs(elem,'mesh_term')
            elem = getElementTag(root, 'intervention_browse')
            addSqlArgs(elem, 'mesh_term')
            addSqlArgs(root,'keyword')


            sqlArgs = tuple(sqlElements)
            tableName = 'clinicaltrials'
            argNumber = len(sqlArgs)
            insertIntoDatabase(tableName,argNumber,sqlArgs)

            #print sqlArgs
            """for s in sqlElements:
                print s
            print len(sqlElements)"""

            iter+=1
