from gensim.utils import simple_preprocess
from gensim.models import word2vec
import os
import logging

class MySentences(object):
    def __init__(self, dirname):
        self.dirname = dirname

    def __iter__(self):
        for fname in os.listdir(self.dirname):
            for line in open(os.path.join(self.dirname, fname)):
                #line = line.lower()
                """line = line.replace('(','')
                line = line.replace(')','')
                line = line.replace('.',' ')
                line = line.replace(',',' ')
                line = line.replace(';', ' ')
                line = line.replace(':', ' ')
                line = line.replace('[', '')
                line = line.replace(']', '')
                line = line.replace('{', '')
                line = line.replace('}', '')
                line = line.replace('"', '')
                line = line.replace("'", '')
                line = line.split()"""
                line = line.replace(' KIT ',' gene_kit ')
                yield simple_preprocess(line)

logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
sentences = MySentences('L:/Python_projects/TREC_2019/2018w2v')  # a memory-friendly iterator
model = word2vec.Word2Vec(sentences, size=400, window=20, min_count=1, workers=4)
model.wv.save_word2vec_format('L:/Python_projects/TREC_2019/VSM/2018_clintr_simplepreprocess_lower_kit_genekit_400_window20_min_count1.bin', binary=True)