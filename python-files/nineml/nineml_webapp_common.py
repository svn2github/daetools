import os, sys, traceback

_css = """
    <style type="text/css">
        body 
        {
            padding:0;
            margin:20px;
            color:#333;
            background: white;
            font:14px sans-serif Tahoma;
            margin-left: auto; 
            margin-right: auto; 
            width: 700px;
        }
        form 
        {
            margin:0;
            padding:0;
        }
        fieldset 
        { 
            margin:1em 0;
            border:none;
            border-top:1px solid #ccc;
        }
        textarea
        {
             border: 1px solid LightGray;
        }
        input
        {
             border: 1px solid LightGray;
        }
        input:hover 
        {
            border: 1px solid Gray;
            background: white;
        }
        textarea:hover 
        {
            border: 1px solid Gray;
            background: white;
        }
        select
        {
            color: #202020; 
            background-color: White;
            width: 200px;
        }
        /*
        input:focus,textarea:focus 
        {
            background:#efefef;
            color:#000;
        }
        */
        
        form fieldset 
        {
            margin-bottom: 10px;
        }
        form legend 
        {
            padding: 0 2px;
            font-weight: bold;
        }
        form label 
        {
            display: inline-block;
            line-height: 1.8;
            vertical-align: top;
        }
        form fieldset ol 
        {
            margin: 0;
            padding: 0;
        }
        form fieldset li 
        {
            list-style: disc;
            padding: 0px;
            margin: 0;
        }
        form fieldset fieldset 
        {
            border: none;
            margin: 3px 0 0;
        }
        form fieldset fieldset legend 
        {
            padding: 0 0 5px;
            font-weight: normal;
        }
        form fieldset fieldset label 
        {
            display: block;
            width: auto;
        }
        form em 
        {
            font-weight: bold;
            font-style: normal;
            color: #f00;
        }
        form label 
        {
            width: 150px; /* Width of labels */
        }
        form fieldset fieldset label 
        {
            margin-left: 153px; /* Width plus 3 (html space) */
        }
    </style>
"""

def getInitialPage():
    html = """
    <html>
        <head>
            CSS_STYLES
        </head>
        <body>
            <form action="nineml-webapp" method="post">
                <p>
                    NineML testable component name:<br/>
                    <input type="text" name="TestableComponent" value="hierachical_iaf_1coba"/>
                </p>
                <p>
                    <!-- 
                    <input type="checkbox" name="AddTest" checked/> 
                    <label for="AddTest" style="width : 250;">Include a test in the repport (optional) </label> <br/>
                    -->
                    Initial values (in JSON format):<br/>
                    <textarea name="InitialValues" rows="20" cols="80">
{
  "timeHorizon": 1.0, 
  "reportingInterval": 0.001, 
  "initial_conditions": {
    "iaf_1coba.iaf.tspike": -1e+99, 
    "iaf_1coba.iaf.V": -0.06, 
    "iaf_1coba.cobaExcit.g": 0.0
  }, 
  "parameters": {
    "iaf_1coba.iaf.gl": 50.0, 
    "iaf_1coba.cobaExcit.vrev": 0.0, 
    "iaf_1coba.cobaExcit.q": 3.0, 
    "iaf_1coba.iaf.vreset": -0.06, 
    "iaf_1coba.cobaExcit.tau": 5.0, 
    "iaf_1coba.iaf.taurefrac": 0.008, 
    "iaf_1coba.iaf.vthresh": -0.04, 
    "iaf_1coba.iaf.vrest": -0.06, 
    "iaf_1coba.iaf.cm": 1.0
  }, 
  "variables_to_report": {
    "iaf_1coba.cobaExcit.I": true, 
    "iaf_1coba.iaf.V": true
  }, 
  "event_ports_expressions": {
    "iaf_1coba.cobaExcit.spikeinput": "0.05, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90"
  }, 
  "active_regimes": {
    "iaf_1coba.cobaExcit": "cobadefaultregime", 
    "iaf_1coba.iaf": "subthresholdregime"
  }, 
  "analog_ports_expressions": {}
}
                </textarea>
                </p>
                <input type="submit" name="__NINEML_WEBAPP_ACTION__" value="Generate report" />
                <input type="submit" name="__NINEML_WEBAPP_ACTION__" value="Add test" />
            </form>
        </body>
        </html>
    """
    return html.replace('CSS_STYLES', _css)

def getSetupDataForm():
    html = """
    <form action="nineml-webapp" method="post">
        <h1>Test NineML component: {0}</h1>
        {1}
        <input type="hidden" name="__NINEML_WEBAPP_ID__" value="{2}"/>
        <input type="hidden" name="__NINEML_WEBAPP_ACTION__" value="Generate report with tests" />
        <br/>
        
        <input type="submit" value="Generate report" />
    </form>
    """
    return html

def createResultPage(content):
    html =  """
    <html>
        <head>
            {0}
        </head>
        <body>
            <pre>
                {1}
            </pre>
        </body>
    </html>
    """
    return html.format(_css, content)

def createSetupDataPage(content):
    html =  """
    <html>
        <head>
            {0}
        </head>
        <body>
            {1}
        </body>
    </html>
    """
    return html.format(_css, content)

def createErrorPage(error, trace_back, additional_data = ''):
    html =  """
    <html>
        <head>
            {0}
        </head>
        <body>
            <pre>Error occurred:\n  {1}</pre>
            <pre>{2}</pre>
            <pre>{3}</pre>
        </body>
    </html>
    """
    errorDescription = ''
    messages = traceback.format_tb(trace_back)
    for msg in messages:
        errorDescription += msg
        
    return html.format(_css, error, errorDescription, additional_data)
