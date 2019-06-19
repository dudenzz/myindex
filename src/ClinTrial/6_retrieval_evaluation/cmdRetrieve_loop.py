import os

def getEval():
    toSave = ''
    for line in evalFile:

        line = line.split('\t')
        if line[0] == 'Rprec':
            toSave += terrierResFileName + '\t' + terrierRankFunc + '\t'
            toSave += line[1] + '\t' + line[2].replace('\n', '') + '\t'
            if execution=='':
                toSave+='no'+'\t'
            else:
                toSave += 'yes' + '\t'
        if line[0] == 'P_5':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_10':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_15':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_20':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_30':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_100':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_200':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_500':
            toSave += line[2].replace('\n', '') + '\t'
        if line[0] == 'P_1000':
            toSave += line[2].replace('\n', '') + '\n'
    return toSave







executions = ['',
              ' -Dtrec.matching=JforestsModelMatching,FatFeaturedScoringMatching,org.terrier.matching.daat.FatFull -Dfat.featured.scoring.matching.features=FILE -Dfat.featured.scoring.matching.features.file=M:\\terrier\\trec_2017\\terrier-core-4.2\\etc\\features.list -Dfat.matching.learned.jforest.model=M:\\terrier\\trec_2017\\terrier-core-4.2\\bin\\ensemble.txt -Dfat.matching.learned.jforest.statistics=M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\results\\jforests-feature-stats.txt -Dproximity.dependency.type=SD']


terrierPath = 'M:\\terrier\\trec_2017\\terrier-core-4.2\\bin\\'
terrierScript = 'trec_terrier -r -Dtrec.model='
terrierRankFunc = 'BB2'

queriesPath = 'M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\queries_2018'

terrierQueryPath = ' -Dtrec.topics=M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\queries_2018\\'
#terrierQueryFile = 'w2v_korhonen_0.85_w1_160_w2_2.txt'

terrierResFile = ' -Dtrec.results.file='


terrierResPath = 'M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\results\\'


terrierEvalScript = 'trec_terrier -e -p '
terrierEvalShortScript = 'trec_terrier -e '
terrierQrels = ' -Dtrec.qrels=M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\qrels2018'

terrierLearnToRank = ' -Dtrec.matching=JforestsModelMatching,FatFeaturedScoringMatching,org.terrier.matching.daat.FatFull -Dfat.featured.scoring.matching.features=FILE -Dfat.featured.scoring.matching.features.file=M:\\terrier\\trec_2017\\terrier-core-4.2\\etc\\features.list -Dfat.matching.learned.jforest.model=M:\\terrier\\trec_2017\\terrier-core-4.2\\bin\\ensemble.txt -Dfat.matching.learned.jforest.statistics=M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\results\\jforests-feature-stats.txt -Dproximity.dependency.type=SD'


pythonPath = 'L:\\Python27_X64\\python.exe '
ageGenderScriptPath = 'L:\\Python_projects\\TREC_2019\\retrieveCheckAgeGender.py'


for execution in executions:
    print 'Calculating'+execution
    #Retrieval
    terrierLearnToRank=execution
    for terrierQueryFile in os.listdir(queriesPath):
        terrierResFileName = terrierQueryFile.replace('.txt', '.res')
        toExecute = terrierPath+terrierScript+terrierRankFunc+terrierResFile+terrierResFileName+terrierQueryPath+terrierQueryFile+terrierLearnToRank


        print toExecute

        os.system(toExecute)


        #Filter age gender

        toExecute = pythonPath+ageGenderScriptPath + ' ' + terrierResFileName
        os.system(toExecute)


        #Evaluate
        #toExecute = 'M:\\terrier\\trec_2017\\terrier-core-4.2\\bin\\trec_terrier -e testRes.res -Dtrec.qrels=M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\qrels2017'
        toExecute = terrierPath+terrierEvalScript+terrierResFileName+terrierQrels
        #print toExecute
        os.system(toExecute)
        #print terrierEvalFilename

        terrierEvalFilename = terrierResPath + terrierResFileName.replace('.res', '.eval')
        evalFile = open(terrierEvalFilename)
        resFile = open('res_comparison_2018_genekit.txt','a')


        toSave = getEval()
        resFile.write(toSave)
        evalFile.close()
        resFile.close()

        toExecute = terrierPath+terrierEvalShortScript+terrierResFileName+terrierQrels
        os.system(toExecute)
        evalFile = open(terrierEvalFilename)
        resFile = open('res_comparison_short_2018_genekit.txt','a')
        toSave = getEval()
        resFile.write(toSave)
        evalFile.close()
        resFile.close()









