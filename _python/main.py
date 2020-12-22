





class WavContainer:

    def __init__(self, filepath: str):
        self.path = filepath

        self.__init_header()


    def __init_header(self):
        """
        Will init file and header
        """

        with open(self.path, 'wb') as wav_file:
            wav_file.write( bytes('RIFF', encoding='utf8') )
            wav_file.write( bytes(4) )
            wav_file.write( bytes([12]) )





WC = WavContainer('test-output-py.bin')