# public
Publicly shared applications and examples.

![alt text](path/img.png)



# Build.props

Create a Build.props file at the root directory of the repo that provides the application that is currently being built.

    <?xml version="1.0" encoding="utf-8"?>
    <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      <ImportGroup Label="PropertySheets" />
      <PropertyGroup Label="UserMacros">
        <TargetBuildApplication>Example</TargetBuildApplication>
      </PropertyGroup>
      <PropertyGroup />
      <ItemDefinitionGroup />
      <ItemGroup>
        <BuildMacro Include="TargetBuildApplication">
          <Value>$(TargetBuildApplication)</Value>
          <EnvironmentVariable>true</EnvironmentVariable>
        </BuildMacro>
      </ItemGroup>
    </Project>