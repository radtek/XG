#ifndef __SVN_VERSION_H__
#define __SVN_VERSION_H__

#define SGSPJ_SVN_VERSION     "$WCREV$"
#define SGSPJ_SVN_MODIFIED    "$WCMODS?Modified:Not modified$"
#define SGSPJ_SVN_Date        "$WCDATE$"
#define SGSPJ_SVN_Range       "$WCRANGE$"
#define SGSPJ_SVN_Mixed       "$WCMIXED?Mixed revision WC:Not mixed$"
#define SGSPJ_SVN_URL         "$WCURL$"

//#if $WCMODS?1:0$
//#error Source is modified
//#endif

#endif //__SVN_VERSION_H__