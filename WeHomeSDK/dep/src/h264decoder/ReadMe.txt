1. ����Ŀ��Linuxƽ̨�±���
2. NDK��װ�ú��Ƚ���standalone���뻷����������벻ͨ��
   python make_standalone_toolchain.py --api 21 --install-dir /home/ffmpeg/work/toolchain/android/linux-x86_64/ndk-r16/android-21/arm --arch arm --stl libc++ --force
3. �޸�ffmpeg_build_android.sh�е�standalong������Ŀ¼
4. ִ��ffmpeg_build_android.sh����ffmpeg��̬��
5. jniĿ¼��ִ�� ndk-build ���ɽ����