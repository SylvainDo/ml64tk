type StringBase = 2 | 8 | 10 | 16;
type BitwiseWidth = 8 | 16 | 32 | 64;

type LargeIntegerOperand = LargeInteger | number | string | BigInt | [number /* lowPart */, number /* highPart */] | [string, StringBase];

interface LargeInteger {
    toNumber(): number;
    toString(): string;
    toString(base: number): string;
    toBigInt(): BigInt;

    lowPart: number; // get, set
    highPart: number; // get, set
    bytes: ArrayBuffer; // get

    neg(): LargeInteger;
    add(rhs: LargeIntegerOperand): LargeInteger;
    sub(rhs: LargeIntegerOperand): LargeInteger;
    mul(rhs: LargeIntegerOperand): LargeInteger;
    div(rhs: LargeIntegerOperand): LargeInteger;
    mod(rhs: LargeIntegerOperand): LargeInteger;
    band(rhs: LargeIntegerOperand): LargeInteger;
    bor(rhs: LargeIntegerOperand): LargeInteger;
    xor(rhs: LargeIntegerOperand): LargeInteger;
    not(): LargeInteger;
    shl(shift: number): LargeInteger;
    shr(shift: number): LargeInteger;
    eq(rhs: LargeIntegerOperand): boolean;
    neq(rhs: LargeIntegerOperand): boolean;
    lt(rhs: LargeIntegerOperand): boolean;
    lte(rhs: LargeIntegerOperand): boolean;
    gt(rhs: LargeIntegerOperand): boolean;
    gte(rhs: LargeIntegerOperand): boolean;
    bswap(): LargeInteger;
    bswap(width: BitwiseWidth): LargeInteger;
    bceil(): LargeInteger;
    bfloor(): LargeInteger;
    bwidth(): number;
    rotl(shift: number): LargeInteger;
    rotl(shift: number, width: BitwiseWidth): LargeInteger;
    rotr(shift: number): LargeInteger;
    rotr(shift: number, width: BitwiseWidth): LargeInteger;
    countlZero(): number;
    countlOne(): number;
    countrZero(): number;
    countrOne(): number;
    popCount(): number;
}

interface LargeIntegerConstructors {
    new(): LargeInteger;
    new(value: LargeIntegerOperand): LargeInteger;
}

export var SignedLargeInteger: LargeIntegerConstructors;
export var UnsignedLargeInteger: LargeIntegerConstructors;

interface OpaquePointer {
    equals(other: OpaquePointer): boolean;
    rawPointer: bigint;
    asPointer(): bigint | undefined;
    asObject(): object | undefined;
    release(): void;
}

export var OpaquePointer: {
    fromPointer(value: bigint): OpaquePointer;
    fromObject(value: object): OpaquePointer;
};

interface Vec2 {
    x: number,
    y: number
}

interface Vec3 {
    x: number,
    y: number,
    z: number
}

interface Vec4 {
    x: number,
    y: number,
    z: number,
    w: number
}

export module Audio {
    const enum SoundSourceStatus {
        Stopped,
        Paused,
        Playing
    }

    interface SoundSource {
        pitch: number;
        volume: number;
        position: Vec3;
        relativeToListener: boolean;
        minDistance: number;
        attenuation: number;
        play(): void;
        pause(): void;
        stop(): void;
        status: SoundSourceStatus;
    }

    interface SoundStream extends SoundSource {
        channelCount: number;
        sampleRate: number;
        playingOffset: number;
        loop: boolean;
    }

    interface SoundBuffer {
        equals(other: SoundBuffer): boolean;
        loadFromFile(filename: string): void;
        loadFromMemory(data: Uint8Array): void;
        loadFromSamples(samples: Int16Array, channelCount: number, sampleRate: number): void;
        saveToFile(filename: string): void;
        samples: Int16Array;
        sampleCount: bigint;
        sampleRate: number;
        channelCount: number;
        duration: number;
    }

    var SoundBuffer: {
        new(): SoundBuffer;
    };

    interface Sound extends SoundSource {
        equals(other: Sound): boolean;
        buffer: SoundBuffer | undefined;
        loop: boolean;
        playingOffset: number;
    }

    var Sound: {
        new(): Sound;
    };

    interface TimeSpan {
        offset: number;
        length: number;
    }

    interface Music extends SoundStream {
        equals(other: Music): boolean;
        openFromFile(filename: string): void;
        openFromMemory(data: Uint8Array): void;
        duration: number;
        loopPoints: TimeSpan;
    }

    var Music: {
        new(): Music;
    };

    var Listener: {
        globalVolume: number;
        position: Vec3;
        direction: Vec3;
        upVector: Vec3;
    };
}

export module ImGui {
    const enum WindowFlags {
        None = 0,
        NoTitleBar = 1 << 0,
        NoResize = 1 << 1,
        NoMove = 1 << 2,
        NoScrollbar = 1 << 3,
        NoScrollWithMouse = 1 << 4,
        NoCollapse = 1 << 5,
        AlwaysAutoResize = 1 << 6,
        NoBackground = 1 << 7,
        NoSavedSettings = 1 << 8,
        NoMouseInputs = 1 << 9,
        MenuBar = 1 << 10,
        HorizontalScrollbar = 1 << 11,
        NoFocusOnAppearing = 1 << 12,
        NoBringToFrontOnFocus = 1 << 13,
        AlwaysVerticalScrollbar = 1 << 14,
        AlwaysHorizontalScrollbar = 1 << 15,
        AlwaysUseWindowPadding = 1 << 16,
        NoNavInputs = 1 << 18,
        NoNavFocus = 1 << 19,
        UnsavedDocument = 1 << 20,
        NoDocking = 1 << 21,
        NoNav = NoNavInputs | NoNavFocus,
        NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
        NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus
    }

    const enum InputTextFlags {
        None = 0,
        CharsDecimal = 1 << 0,
        CharsHexadecimal = 1 << 1,
        CharsUppercase = 1 << 2,
        CharsNoBlank = 1 << 3,
        AutoSelectAll = 1 << 4,
        EnterReturnsTrue = 1 << 5,
        CallbackCompletion = 1 << 6,
        CallbackHistory = 1 << 7,
        CallbackAlways = 1 << 8,
        CallbackCharFilter = 1 << 9,
        AllowTabInput = 1 << 10,
        CtrlEnterForNewLine = 1 << 11,
        NoHorizontalScroll = 1 << 12,
        AlwaysOverwrite = 1 << 13,
        ReadOnly = 1 << 14,
        Password = 1 << 15,
        NoUndoRedo = 1 << 16,
        CharsScientific = 1 << 17,
        CallbackResize = 1 << 18,
        CallbackEdit = 1 << 19
    }

    const enum TreeNodeFlags {
        None = 0,
        Selected = 1 << 0,
        Framed = 1 << 1,
        AllowItemOverlap = 1 << 2,
        NoTreePushOnOpen = 1 << 3,
        NoAutoOpenOnLog = 1 << 4,
        DefaultOpen = 1 << 5,
        OpenOnDoubleClick = 1 << 6,
        OpenOnArrow = 1 << 7,
        Leaf = 1 << 8,
        Bullet = 1 << 9,
        FramePadding = 1 << 10,
        SpanAvailWidth = 1 << 11,
        SpanFullWidth = 1 << 12,
        NavLeftJumpsBackHere = 1 << 13,
        // NoScrollOnOpen = 1 << 14,
        CollapsingHeader = Framed | NoTreePushOnOpen | NoAutoOpenOnLog
    }

    const enum PopupFlags {
        None = 0,
        MouseButtonLeft = 0,
        MouseButtonRight = 1,
        MouseButtonMiddle = 2,
        NoOpenOverExistingPopup = 1 << 5,
        NoOpenOverItems = 1 << 6,
        AnyPopupId = 1 << 7,
        AnyPopupLevel = 1 << 8,
        AnyPopup = AnyPopupId | AnyPopupLevel
    }

    const enum SelectableFlags {
        None = 0,
        DontClosePopups = 1 << 0,
        SpanAllColumns = 1 << 1,
        AllowDoubleClick = 1 << 2,
        Disabled = 1 << 3,
        AllowItemOverlap = 1 << 4
    }

    const enum ComboFlags {
        None = 0,
        PopupAlignLeft = 1 << 0,
        HeightSmall = 1 << 1,
        HeightRegular = 1 << 2,
        HeightLarge = 1 << 3,
        HeightLargest = 1 << 4,
        NoArrowButton = 1 << 5,
        NoPreview = 1 << 6
    }

    const enum TabBarFlags {
        None = 0,
        Reorderable = 1 << 0,
        AutoSelectNewTabs = 1 << 1,
        TabListPopupButton = 1 << 2,
        NoCloseWithMiddleMouseButton = 1 << 3,
        NoTabListScrollingButtons = 1 << 4,
        NoTooltip = 1 << 5,
        FittingPolicyResizeDown = 1 << 6,
        FittingPolicyScroll = 1 << 7
    }

    const enum TabItemFlags {
        None = 0,
        UnsavedDocument = 1 << 0,
        SetSelected = 1 << 1,
        NoCloseWithMiddleMouseButton = 1 << 2,
        NoPushId = 1 << 3,
        NoTooltip = 1 << 4,
        NoReorder = 1 << 5,
        Leading = 1 << 6,
        Trailing = 1 << 7
    }

    const enum TableFlags {
        None = 0,
        Resizable = 1 << 0,
        Reorderable = 1 << 1,
        Hideable = 1 << 2,
        Sortable = 1 << 3,
        NoSavedSettings = 1 << 4,
        ContextMenuInBody = 1 << 5,
        RowBg = 1 << 6,
        BordersInnerH = 1 << 7,
        BordersOuterH = 1 << 8,
        BordersInnerV = 1 << 9,
        BordersOuterV = 1 << 10,
        BordersH = BordersInnerH | BordersOuterH,
        BordersV = BordersInnerV | BordersOuterV,
        BordersInner = BordersInnerV | BordersInnerH,
        BordersOuter = BordersOuterV | BordersOuterH,
        Borders = BordersInner | BordersOuter,
        NoBordersInBody = 1 << 11,
        NoBordersInBodyUntilResize = 1 << 12,
        SizingFixedFit = 1 << 13,
        SizingFixedSame = 2 << 13,
        SizingStretchProp = 3 << 13,
        SizingStretchSame = 4 << 13,
        NoHostExtendX = 1 << 16,
        NoHostExtendY = 1 << 17,
        NoKeepColumnsVisible = 1 << 18,
        PreciseWidths = 1 << 19,
        NoClip = 1 << 20,
        PadOuterX = 1 << 21,
        NoPadOuterX = 1 << 22,
        NoPadInnerX = 1 << 23,
        ScrollX = 1 << 24,
        ScrollY = 1 << 25,
        SortMulti = 1 << 26,
        SortTristate = 1 << 27,
    }

    const enum TableColumnFlags {
        None = 0,
        Disabled = 1 << 0,
        DefaultHide = 1 << 1,
        DefaultSort = 1 << 2,
        WidthStretch = 1 << 3,
        WidthFixed = 1 << 4,
        NoResize = 1 << 5,
        NoReorder = 1 << 6,
        NoHide = 1 << 7,
        NoClip = 1 << 8,
        NoSort = 1 << 9,
        NoSortAscending = 1 << 10,
        NoSortDescending = 1 << 11,
        NoHeaderLabel = 1 << 12,
        NoHeaderWidth = 1 << 13,
        PreferSortAscending = 1 << 14,
        PreferSortDescending = 1 << 15,
        IndentEnable = 1 << 16,
        IndentDisable = 1 << 17,
        IsEnabled = 1 << 24,
        IsVisible = 1 << 25,
        IsSorted = 1 << 26,
        IsHovered = 1 << 27
    }

    const enum TableRowFlags {
        None = 0,
        Headers = 1 << 0
    }

    const enum TableBgTarget {
        None = 0,
        RowBg0 = 1,
        RowBg1 = 2,
        CellBg = 3
    }

    const enum FocusedFlags {
        None = 0,
        ChildWindows = 1 << 0,
        RootWindow = 1 << 1,
        AnyWindow = 1 << 2,
        NoPopupHierarchy = 1 << 3,
        DockHierarchy = 1 << 4,
        RootAndChildWindows = RootWindow | ChildWindows,
    }

    const enum HoveredFlags {
        None = 0,
        ChildWindows = 1 << 0,
        RootWindow = 1 << 1,
        AnyWindow = 1 << 2,
        NoPopupHierarchy = 1 << 3,
        DockHierarchy = 1 << 4,
        AllowWhenBlockedByPopup = 1 << 5,
        // AllowWhenBlockedByModal = 1 << 6,
        AllowWhenBlockedByActiveItem = 1 << 7,
        AllowWhenOverlapped = 1 << 8,
        AllowWhenDisabled = 1 << 9,
        NoNavOverride = 1 << 10,
        RectOnly = AllowWhenBlockedByPopup | AllowWhenBlockedByActiveItem | AllowWhenOverlapped,
        RootAndChildWindows = RootWindow | ChildWindows
    }

    const enum DockNodeFlags {
        None = 0,
        KeepAliveOnly = 1 << 0,
        // NoCentralNode = 1 << 1,
        NoDockingInCentralNode = 1 << 2,
        PassthruCentralNode = 1 << 3,
        NoSplit = 1 << 4,
        NoResize = 1 << 5,
        AutoHideTabBar = 1 << 6
    }

    const enum DragDropFlags {
        None = 0,
        SourceNoPreviewTooltip = 1 << 0,
        SourceNoDisableHover = 1 << 1,
        SourceNoHoldToOpenOthers = 1 << 2,
        SourceAllowNullID = 1 << 3,
        SourceExtern = 1 << 4,
        SourceAutoExpirePayload = 1 << 5,
        AcceptBeforeDelivery = 1 << 10,
        AcceptNoDrawDefaultRect = 1 << 11,
        AcceptNoPreviewTooltip = 1 << 12,
        AcceptPeekOnly = AcceptBeforeDelivery | AcceptNoDrawDefaultRect
    }

    const enum Dir {
        None = -1,
        Left = 0,
        Right = 1,
        Up = 2,
        Down = 3
    }

    const enum SortDirection {
        None = 0,
        Ascending = 1,
        Descending = 2
    }

    const enum Key {
        None = 0,
        Tab = 512,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        PageUp,
        PageDown,
        Home,
        End,
        Insert,
        Delete,
        Backspace,
        Space,
        Enter,
        Escape,
        LeftCtrl, LeftShift, LeftAlt, LeftSuper,
        RightCtrl, RightShift, RightAlt, RightSuper,
        Menu,
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        A, B, C, D, E, F, G, H, I, J,
        K, L, M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z,
        F1, F2, F3, F4, F5, F6,
        F7, F8, F9, F10, F11, F12,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        Semicolon,
        Equal,
        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4,
        Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        GamepadStart,
        GamepadBack,
        GamepadFaceLeft,
        GamepadFaceRight,
        GamepadFaceUp,
        GamepadFaceDown,
        GamepadDpadLeft,
        GamepadDpadRight,
        GamepadDpadUp,
        GamepadDpadDown,
        GamepadL1,
        GamepadR1,
        GamepadL2,
        GamepadR2,
        GamepadL3,
        GamepadR3,
        GamepadLStickLeft,
        GamepadLStickRight,
        GamepadLStickUp,
        GamepadLStickDown,
        GamepadRStickLeft,
        GamepadRStickRight,
        GamepadRStickUp,
        GamepadRStickDown,
        ModCtrl, ModShift, ModAlt, ModSuper
    }

    const enum ModFlags {
        None = 0,
        Ctrl = 1 << 0,
        Shift = 1 << 1,
        Alt = 1 << 2,
        Super = 1 << 3
    }

    const enum ConfigFlags {
        None = 0,
        NavEnableKeyboard = 1 << 0,
        NavEnableGamepad = 1 << 1,
        NavEnableSetMousePos = 1 << 2,
        NavNoCaptureKeyboard = 1 << 3,
        NoMouse = 1 << 4,
        NoMouseCursorChange = 1 << 5,
        DockingEnable = 1 << 6,
        ViewportsEnable = 1 << 10,
        DpiEnableScaleViewports = 1 << 14,
        DpiEnableScaleFonts = 1 << 15,
        IsSRGB = 1 << 20,
        IsTouchScreen = 1 << 21
    }

    const enum BackendFlags {
        None = 0,
        HasGamepad = 1 << 0,
        HasMouseCursors = 1 << 1,
        HasSetMousePos = 1 << 2,
        RendererHasVtxOffset = 1 << 3,
        PlatformHasViewports = 1 << 10,
        HasMouseHoveredViewport = 1 << 11,
        RendererHasViewports = 1 << 12
    }

    const enum Col {
        Text,
        TextDisabled,
        WindowBg,
        ChildBg,
        PopupBg,
        Border,
        BorderShadow,
        FrameBg,
        FrameBgHovered,
        FrameBgActive,
        TitleBg,
        TitleBgActive,
        TitleBgCollapsed,
        MenuBarBg,
        ScrollbarBg,
        ScrollbarGrab,
        ScrollbarGrabHovered,
        ScrollbarGrabActive,
        CheckMark,
        SliderGrab,
        SliderGrabActive,
        Button,
        ButtonHovered,
        ButtonActive,
        Header,
        HeaderHovered,
        HeaderActive,
        Separator,
        SeparatorHovered,
        SeparatorActive,
        ResizeGrip,
        ResizeGripHovered,
        ResizeGripActive,
        Tab,
        TabHovered,
        TabActive,
        TabUnfocused,
        TabUnfocusedActive,
        DockingPreview,
        DockingEmptyBg,
        PlotLines,
        PlotLinesHovered,
        PlotHistogram,
        PlotHistogramHovered,
        TableHeaderBg,
        TableBorderStrong,
        TableBorderLight,
        TableRowBg,
        TableRowBgAlt,
        TextSelectedBg,
        DragDropTarget,
        NavHighlight,
        NavWindowingHighlight,
        NavWindowingDimBg,
        ModalWindowDimBg
    }

    const enum StyleVar {
        Alpha,
        DisabledAlpha,
        WindowPadding,
        WindowRounding,
        WindowBorderSize,
        WindowMinSize,
        WindowTitleAlign,
        ChildRounding,
        ChildBorderSize,
        PopupRounding,
        PopupBorderSize,
        FramePadding,
        FrameRounding,
        FrameBorderSize,
        ItemSpacing,
        ItemInnerSpacing,
        IndentSpacing,
        CellPadding,
        ScrollbarSize,
        ScrollbarRounding,
        GrabMinSize,
        GrabRounding,
        TabRounding,
        ButtonTextAlign,
        SelectableTextAlign
    }

    const enum ButtonFlags {
        None = 0,
        MouseButtonLeft = 1 << 0,
        MouseButtonRight = 1 << 1,
        MouseButtonMiddle = 1 << 2
    }

    const enum ColorEditFlags {
        None = 0,
        NoAlpha = 1 << 1,
        NoPicker = 1 << 2,
        NoOptions = 1 << 3,
        NoSmallPreview = 1 << 4,
        NoInputs = 1 << 5,
        NoTooltip = 1 << 6,
        NoLabel = 1 << 7,
        NoSidePreview = 1 << 8,
        NoDragDrop = 1 << 9,
        NoBorder = 1 << 10,
        AlphaBar = 1 << 16,
        AlphaPreview = 1 << 17,
        AlphaPreviewHalf = 1 << 18,
        HDR = 1 << 19,
        DisplayRGB = 1 << 20,
        DisplayHSV = 1 << 21,
        DisplayHex = 1 << 22,
        Uint8 = 1 << 23,
        Float = 1 << 24,
        PickerHueBar = 1 << 25,
        PickerHueWheel = 1 << 26,
        InputRGB = 1 << 27,
        InputHSV = 1 << 28
    }

    const enum SliderFlags {
        None = 0,
        AlwaysClamp = 1 << 4,
        Logarithmic = 1 << 5,
        NoRoundToFormat = 1 << 6,
        NoInput = 1 << 7
    }

    const enum MouseButton {
        Left = 0,
        Right = 1,
        Middle = 2
    }

    const enum MouseCursor {
        None = -1,
        Arrow = 0,
        TextInput,
        ResizeAll,
        ResizeNS,
        ResizeEW,
        ResizeNESW,
        ResizeNWSE,
        Hand,
        NotAllowed
    }

    const enum Cond {
        None = 0,
        Always = 1 << 0,
        Once = 1 << 1,
        FirstUseEver = 1 << 2,
        Appearing = 1 << 3
    }

    interface Style {
        equals(other: Style): boolean;
        alpha: number;
        disabledAlpha: number;
        windowPadding: Vec2;
        windowRounding: number;
        windowBorderSize: number;
        windowMinSize: Vec2;
        windowTitleAlign: Vec2;
        windowMenuButtonPosition: Dir;
        childRounding: number;
        childBorderSize: number;
        popupRounding: number;
        popupBorderSize: number;
        framePadding: Vec2;
        frameRounding: number;
        frameBorderSize: number;
        itemSpacing: Vec2;
        itemInnerSpacing: Vec2;
        cellPadding: Vec2;
        touchExtraPadding: Vec2;
        indentSpacing: number;
        columnsMinSpacing: number;
        scrollbarSize: number;
        scrollbarRounding: number;
        grabMinSize: number;
        grabRounding: number;
        logSliderDeadzone: number;
        tabRounding: number;
        tabBorderSize: number;
        tabMinWidthForCloseButton: number;
        colorButtonPosition: Dir;
        buttonTextAlign: Vec2;
        selectableTextAlign: Vec2;
        displayWindowPadding: Vec2;
        displaySafeAreaPadding: Vec2;
        mouseCursorScale: number;
        antiAliasedLines: boolean;
        antiAliasedLinesUseTex: boolean;
        antiAliasedFill: boolean;
        curveTessellationTol: number;
        circleTessellationMaxError: number;
        getColorAt(index: Col): Vec4;
        setColorAt(index: Col, value: Vec4): void;
        scaleAllSizes(scaleFactor: number): void;
    }

    var Style: {
        new(): Style;
    };

    interface KeyData {
        equals(other: KeyData): boolean;
        down: boolean;
        downDuration: number;
        downDurationPrev: number;
        analogValue: number;
    }

    interface IO {
        equals(other: IO): boolean;
        configFlags: ConfigFlags;
        backendFlags: BackendFlags;
        displaySize: Vec2;
        deltaTime: number;
        iniSavingRate: number;
        iniFilename: string | undefined;
        logFilename: string | undefined;
        mouseDoubleClickTime: number;
        mouseDoubleClickMaxDist: number;
        mouseDragThreshold: number;
        keyRepeatDelay: number;
        keyRepeatRate: number;
        fonts: FontAtlas;
        fontGlobalScale: number;
        fontAllowUserScaling: boolean;
        fontDefault: Font;
        displayFramebufferScale: Vec2;
        configDockingNoSplit: boolean;
        configDockingWithShift: boolean;
        configDockingAlwaysTabBar: boolean;
        configDockingTransparentPayload: boolean;
        configViewportsNoAutoMerge: boolean;
        configViewportsNoTaskBarIcon: boolean;
        configViewportsNoDecoration: boolean;
        configViewportsNoDefaultParent: boolean;
        mouseDrawCursor: boolean;
        configMacOSXBehaviors: boolean;
        configInputTrickleEventQueue: boolean;
        configInputTextCursorBlink: boolean;
        configInputTextEnterKeepActive: boolean;
        configDragClickToInputText: boolean;
        configWindowsResizeFromEdges: boolean;
        configWindowsMoveFromTitleBarOnly: boolean;
        configMemoryCompactTimer: number;
        backendPlatformName: string | undefined;
        backendRendererName: string | undefined;
        addKeyEvent(key: number, down: boolean): void;
        addKeyAnalogEvent(key: number, down: boolean, v: number): void;
        addMousePosEvent(x: number, y: number): void;
        addMouseButtonEvent(button: number, down: boolean): void;
        addMouseWheelEvent(whX: number, whY: number): void;
        addMouseViewportEvent(id: number): void;
        addFocusEvent(focused: boolean): void;
        addInputCharacter(c: number): void;
        addInputCharacterUtf16(c: number): void;
        addInputCharactersUtf8(c: string): void;
        setKeyEventNativeData(key: number, nativeKeycode: number, nativeScancode: number): void;
        setAppAcceptingEvents(acceptingEvents: boolean): void;
        clearInputCharacters(): void;
        clearInputKeys(): void;
        wantCaptureMouse: boolean;
        wantCaptureKeyboard: boolean;
        wantTextInput: boolean;
        wantSetMousePos: boolean;
        wantSaveIniSettings: boolean;
        navActive: boolean;
        navVisible: boolean;
        framerate: number;
        metricsRenderVertices: number;
        metricsRenderIndices: number;
        metricsRenderWindows: number;
        metricsActiveWindows: number;
        metricsActiveAllocations: number;
        mouseDelta: Vec2;

        internalMousePos: Vec2;
        internalGetMouseDownAt(index: number): boolean;
        internalSetMouseDownAt(index: number, value: boolean): void;
        internalMouseWheel: number;
        internalMouseWheelH: number;
        internalMouseHoveredViewport: number;
        internalKeyCtrl: boolean;
        internalKeyShift: boolean;
        internalKeyAlt: boolean;
        internalKeySuper: boolean;
        internalKeyMods: ModFlags;
        internalGetKeyDataAt(index: Key): KeyData;
        internalWantCaptureMouseUnlessPopupClose: boolean;
        internalMousePosPrev: Vec2;
        internalGetMouseClickedPosAt(index: number): Vec2;
        internalSetMouseClickedPosAt(index: number, value: Vec2): void;
        internalGetMouseClickedTimeAt(index: number): number;
        internalSetMouseClickedTimeAt(index: number, value: number): void;
        internalGetMouseClickedAt(index: number): boolean;
        internalSetMouseClickedAt(index: number, value: boolean): void;
        internalGetMouseDoubleClickedAt(index: number): boolean;
        internalSetMouseDoubleClickedAt(index: number, value: boolean): void;
        internalGetMouseClickedCountAt(index: number): number;
        internalSetMouseClickedCountAt(index: number, value: number): void;
        internalGetMouseClickedLastCountAt(index: number): number;
        internalSetMouseClickedLastCountAt(index: number, value: number): void;
        internalGetMouseReleasedAt(index: number): boolean;
        internalSetMouseReleasedAt(index: number, value: boolean): void;
        internalGetMouseDownOwnedAt(index: number): boolean;
        internalSetMouseDownOwnedAt(index: number, value: boolean): void;
        internalGetMouseDownOwnedUnlessPopupCloseAt(index: number): boolean;
        internalSetMouseDownOwnedUnlessPopupCloseAt(index: number, value: boolean): void;
        internalGetMouseDownDurationAt(index: number): number;
        internalSetMouseDownDurationAt(index: number, value: number): void;
        internalGetMouseDownDurationPrevAt(index: number): number;
        internalSetMouseDownDurationPrevAt(index: number, value: number): void;
        internalGetMouseDragMaxDistanceAbsAt(index: number): Vec2;
        internalSetMouseDragMaxDistanceAbsAt(index: number, value: Vec2): void;
        internalGetMouseDragMaxDistanceSqrAt(index: number): number;
        internalSetMouseDragMaxDistanceSqrAt(index: number, value: number): void;
        internalPenPressure: number;
        internalAppFocusLost: boolean;
        internalAppAcceptingEvents: boolean;
        internalBackendUsingLegacyKeyArrays: number;
        internalBackendUsingLegacyNavInputArray: boolean;
        internalInputQueueSurrogate: number;
        internalInputQueueCharacters: ArrayBuffer;
    }

    interface InputTextCallbackData {
        equals(other: InputTextCallbackData): boolean;
        eventFlag: InputTextFlags;
        flags: InputTextFlags;
        eventChar: number;
        eventKey: Key;
        buf: string;
        cursorPos: number;
        selectionStart: number;
        selectionEnd: number;
        deleteChars(pos: number, bytesCount: number): void;
        insertChars(pos: number, text: string): void;
        selectAll(): void;
        clearSelection(): void;
        hasSelection(): boolean;
    }

    interface SizeCallbackData {
        equals(other: SizeCallbackData): boolean;
        pos: Vec2;
        currentSize: Vec2;
        desiredSize: Vec2;
    }

    interface WindowClass {
        equals(other: WindowClass): boolean;
        classId: number;
        parentViewportId: number;
        viewportFlagsOverrideSet: ViewportFlags;
        viewportFlagsOverrideClear: ViewportFlags;
        tabItemFlagsOverrideSet: TabItemFlags;
        dockNodeFlagsOverrideSet: DockNodeFlags;
        dockingAlwaysTabBar: boolean;
        dockingAllowUnclassed: boolean;
    }

    var WindowClass: {
        new(): WindowClass;
    };

    interface Payload {
        equals(other: Payload): boolean;
        data: ArrayBuffer;
        clear(): void;
        isDataType(type: string): boolean;
        isPreview(): boolean;
        isDelivery(): boolean;
    }

    interface TableColumnSortSpecs {
        equals(other: TableColumnSortSpecs): boolean;
        columnUserId: number;
        columnIndex: number;
        sortOrder: number;
        sortDirection: SortDirection;
    }

    interface TableSortSpecs {
        equals(other: TableSortSpecs): boolean;
        getSpecsAt(index: number): TableColumnSortSpecs;
        specsCount: number;
        specsDirty: boolean;
    }

    const enum DrawFlags {
        None = 0,
        Closed = 1 << 0,
        RoundCornersTopLeft = 1 << 4,
        RoundCornersTopRight = 1 << 5,
        RoundCornersBottomLeft = 1 << 6,
        RoundCornersBottomRight = 1 << 7,
        RoundCornersNone = 1 << 8,
        RoundCornersTop = RoundCornersTopLeft | RoundCornersTopRight,
        RoundCornersBottom = RoundCornersBottomLeft | RoundCornersBottomRight,
        RoundCornersLeft = RoundCornersBottomLeft | RoundCornersTopLeft,
        RoundCornersRight = RoundCornersBottomRight | RoundCornersTopRight,
        RoundCornersAll = RoundCornersTopLeft | RoundCornersTopRight | RoundCornersBottomLeft | RoundCornersBottomRight
    }

    const enum DrawListFlags {
        None = 0,
        AntiAliasedLines = 1 << 0,
        AntiAliasedLinesUseTex = 1 << 1,
        AntiAliasedFill = 1 << 2,
        AllowVtxOffset = 1 << 3
    }

    const enum DrawFlipFlags {
        None = 0x00000000,
        Horizontal = 0x00000001,
        Vertical = 0x00000002
    }

    interface DrawSpriteOptions {
        src?: Vec4;
        dst?: Vec4;
        tint?: Vec4;
        flip?: DrawFlipFlags;
        angle?: number;
        offset?: Vec2;
        scale?: Vec2;
        truncVtx?: boolean;
    }

    interface DrawList {
        equals(other: DrawList): boolean;
        flags: DrawListFlags;
        pushClipRect(clipRectMin: Vec2, clipRectMax: Vec2, intersectWithCurrentClipRect?: boolean): void;
        pushClipRectFullScreen(): void;
        popClipRect(): void;
        pushTextureId(textureId: OpaquePointer): void;
        popTextureId(): void;
        getClipRectMin(): Vec2;
        getClipRectMax(): Vec2;
        addLine(p1: Vec2, p2: Vec2, col: Vec4, thickness?: number): void;
        addRect(pMin: Vec2, pMax: Vec2, col: Vec4, rounding?: number, flags?: DrawFlags, thickness?: number): void;
        addRectFilled(pMin: Vec2, pMax: Vec2, col: Vec4, rounding?: number, flags?: DrawFlags): void;
        addRectFilledMultiColor(pMin: Vec2, pMax: Vec2, colUprLeft: Vec4, colUprRight: Vec4, colBotRight: Vec4, colBotLeft: Vec4): void;
        addQuad(p1: Vec2, p2: Vec2, p3: Vec2, p4: Vec2, col: Vec4, thickness?: number): void;
        addQuadFilled(p1: Vec2, p2: Vec2, p3: Vec2, p4: Vec2, col: Vec4): void;
        addTriangle(p1: Vec2, p2: Vec2, p3: Vec2, col: Vec4, thickness?: number): void;
        addTriangleFilled(p1: Vec2, p2: Vec2, p3: Vec2, col: Vec4): void;
        addCircle(center: Vec2, radius: number, col: Vec4, numSegments?: number, thickness?: number): void;
        addCircleFilled(center: Vec2, radius: number, col: Vec4, numSegments?: number): void;
        addNgon(center: Vec2, radius: number, col: Vec4, numSegments: number, thickness?: number): void;
        addNgonFilled(center: Vec2, radius: number, col: Vec4, numSegments: number): void;
        addText(pos: Vec2, col: Vec4, text: string): void;
        addText(font: Font, fontSize: number, pos: Vec2, col: Vec4, text: string, wrapWidth?: number, cpuFineClipRect?: Vec4): void;
        addPolyline(points: Vec2[], col: Vec4, flags?: DrawFlags, thickness?: number): void;
        addConvexPolyFilled(points: Vec2[], col: Vec4): void;
        addBezierCubic(p1: Vec2, p2: Vec2, p3: Vec2, p4: Vec2, col: Vec4, thickness?: number, numSegments?: number): void;
        addBezierQuadratic(p1: Vec2, p2: Vec2, p3: Vec2, col: Vec4, thickness?: number, numSegments?: number): void;
        addImage(userTextureId: OpaquePointer, pMin: Vec2, pMax: Vec2, uvMin?: Vec2, uvMax?: Vec2, col?: Vec4): void;
        addImageQuad(userTextureId: OpaquePointer, p1: Vec2, p2: Vec2, p3: Vec2, p4: Vec2, uv1?: Vec2, uv2?: Vec2, uv3?: Vec2, uv4?: Vec2, col?: Vec4): void;
        addImageRounded(userTextureId: OpaquePointer, pMin: Vec2, pMax: Vec2, uvMin?: Vec2, uvMax?: Vec2, col?: Vec4, rounding?: number, flags?: DrawFlags): void;
        pathClear(): void;
        pathLineTo(pos: Vec2): void;
        pathLineToMergeDuplicate(pos: Vec2): void;
        pathFillConvex(col: Vec4): void;
        pathStroke(col: Vec4, flags?: DrawFlags, thickness?: number): void;
        pathArcTo(center: Vec2, radius: number, aMin: number, aMax: number, numSegments?: number): void;
        pathArcToFast(center: Vec2, radius: number, aMinOf12: number, aMaxOf12: number): void;
        pathBezierCubicCurveTo(p2: Vec2, p3: Vec2, p4: Vec2, numSegments?: number): void;
        pathBezierQuadraticCurveTo(p2: Vec2, p3: Vec2, numSegments?: number): void;
        pathRect(rectMin: Vec2, rectMax: Vec2, rounding?: number, flags?: DrawFlags): void;
        addDrawCmd(): void;
        addSprite(texture: Gfx.Texture, pos: Vec2): void;
        addSpriteEx(texture: Gfx.Texture, options?: DrawSpriteOptions): void;
    }

    const enum FreeTypeBuilderFlags {
        NoHinting = 1 << 0,
        NoAutoHint = 1 << 1,
        ForceAutoHint = 1 << 2,
        LightHinting = 1 << 3,
        MonoHinting = 1 << 4,
        Bold = 1 << 5,
        Oblique = 1 << 6,
        Monochrome = 1 << 7,
        LoadColor = 1 << 8,
        Bitmap = 1 << 9
    }

    interface FontConfig {
        equals(other: FontConfig): boolean;
        fontNo: number;
        sizePixels: number;
        oversampleH: number;
        oversampleV: number;
        pixelSnapH: boolean;
        glyphExtraSpacing: Vec2;
        glyphOffset: Vec2;
        glyphMinAdvanceX: number;
        glyphMaxAdvanceX: number;
        mergeMode: boolean;
        fontBuilderFlags: FreeTypeBuilderFlags;
        rasterizerMultiply: number;
        ellipsisChar: number;
    }

    var FontConfig: {
        new(): FontConfig;
    };

    interface FontGlyphRangesBuilder {
        equals(other: FontGlyphRangesBuilder): boolean;
        clear(): void;
        getBit(n: bigint): boolean;
        setBit(n: bigint): void;
        addChar(c: number): void;
        addText(text: string): void;
        addRanges(ranges: Uint16Array): void;
        build(): Uint16Array;
    }

    var FontGlyphRangesBuilder: {
        new(): FontGlyphRangesBuilder;
    };

    const enum FontAtlasFlags {
        None = 0,
        NoPowerOfTwoHeight = 1 << 0,
        NoMouseCursors = 1 << 1,
        NoBakedLines = 1 << 2
    }

    interface FontAtlas {
        equals(other: FontAtlas): boolean;
        addFontDefault(cfg?: FontConfig): Font;
        addFontFromFile(filename: string, sizePixels: number, cfg?: FontConfig, glyphRanges?: Uint16Array): Font;
        addFontFromMemory(data: ArrayBuffer, sizePixels: number, cfg?: FontConfig, glyphRanges?: Uint16Array): Font;
        clear(): void;
        glyphRangesDefault: Uint16Array;
        glyphRangesKorean: Uint16Array;
        glyphRangesJapanese: Uint16Array;
        glyphRangesChineseFull: Uint16Array;
        glyphRangesChineseSimplifiedCommon: Uint16Array;
        glyphRangesCyrillic: Uint16Array;
        glyphRangesThai: Uint16Array;
        glyphRangesVietnamese: Uint16Array;
    }

    interface Font {
        equals(other: Font): boolean;
        fallbackAdvanceX: number;
        fontSize: number;
        containerAtlas: FontAtlas;
        scale: number;
        ascent: number;
        descent: number;
        metricsTotalSurface: number;
        getCharAdvance(c: number): number;
        calcTextSizeA(size: number, maxWidth: number, wrapWidth: number, text: string): Vec2;
        calcWordWrapPositionA(scale: number, text: string, wrapWidth: number): string;
        renderChar(drawList: DrawList, size: number, pos: Vec2, col: Vec4, c: number): void;
        renderText(drawList: DrawList, size: number, pos: Vec2, col: Vec4, clipRect: Vec4, text: string, wrapWidth?: number, cpuFineClip?: boolean): void;
    }

    const enum ViewportFlags {
        None = 0,
        IsPlatformWindow = 1 << 0,
        IsPlatformMonitor = 1 << 1,
        OwnedByApp = 1 << 2,
        NoDecoration = 1 << 3,
        NoTaskBarIcon = 1 << 4,
        NoFocusOnAppearing = 1 << 5,
        NoFocusOnClick = 1 << 6,
        NoInputs = 1 << 7,
        NoRendererClear = 1 << 8,
        TopMost = 1 << 9,
        Minimized = 1 << 10,
        NoAutoMerge = 1 << 11,
        CanHostOtherWindows = 1 << 12
    }

    interface Viewport {
        equals(other: Viewport): boolean;
        id: number;
        flags: ViewportFlags;
        pos: Vec2;
        size: Vec2;
        workPos: Vec2;
        workSize: Vec2;
        dpiScale: number;
        parentViewportId: number;
        platformHandle: OpaquePointer;
        platformHandleRaw: OpaquePointer;
        getCenter(): Vec2;
        getWorkCenter(): Vec2;
    }

    interface MemoryEditor {
        equals(other: MemoryEditor): boolean;
        open: boolean;
        readOnly: boolean;
        cols: number;
        optShowOptions: boolean;
        optShowDataPreview: boolean;
        optShowHexII: boolean;
        optShowAscii: boolean;
        optGreyOutZeroes: boolean;
        optUpperCaseHex: boolean;
        optMidColsCount: number;
        optAddrDigitsCount: number;
        optFooterExtraHeight: number;
        highlightColor: Vec4;
        drawWindow(title: string, data: ArrayBuffer | OpaquePointer, size: LargeIntegerOperand, baseDisplayAddr?: LargeIntegerOperand): void;
        drawContents(data: ArrayBuffer | OpaquePointer, size: LargeIntegerOperand, baseDisplayAddr?: LargeIntegerOperand): void;
    }

    var MemoryEditor: {
        new(): MemoryEditor;
    };

    type boolRef = boolean[];
    type numberRef = number[];
    type stringRef = string[];

    function getIO(): IO;
    function getStyle(): Style;
    function showDemoWindow(open?: boolRef): void;
    function showMetricsWindow(open?: boolRef): void;
    function showDebugLogWindow(open?: boolRef): void;
    function showStackToolWindow(open?: boolRef): void;
    function showAboutWindow(open?: boolRef): void;
    function showStyleEditor(ref?: Style): void;
    function showStyleSelector(label: string): boolean;
    function showFontSelector(label: string): void;
    function showUserGuide(): void;
    function getVersion(): string;
    function styleColorsDark(dst?: Style): void;
    function styleColorsLight(dst?: Style): void;
    function styleColorsClassic(dst?: Style): void;
    function begin(name: string, open?: boolRef, flags?: WindowFlags): boolean;
    function end(): void;
    function beginChild(id: string, size?: Vec2, border?: boolean, flags?: WindowFlags): boolean;
    function beginChild(id: number, size?: Vec2, border?: boolean, flags?: WindowFlags): boolean;
    function endChild(): void;
    function isWindowAppearing(): boolean;
    function isWindowCollapsed(): boolean;
    function isWindowFocused(flags?: FocusedFlags): boolean;
    function isWindowHovered(flags?: HoveredFlags): boolean;
    function getWindowDrawList(): DrawList;
    function getWindowDpiScale(): number;
    function getWindowPos(): Vec2;
    function getWindowSize(): Vec2;
    function getWindowWidth(): number;
    function getWindowHeight(): number;
    function getWindowViewport(): Viewport;
    function setNextWindowPos(pos: Vec2, cond?: Cond, pivot?: Vec2): void;
    function setNextWindowSize(size: Vec2, cond?: Cond): void;
    function setNextWindowSizeConstraints(sizeMin: Vec2, sizeMax: Vec2, callback?: (data: SizeCallbackData) => void): void;
    function setNextWindowContentSize(size: Vec2): void;
    function setNextWindowCollapsed(collapsed: boolean, cond?: Cond): void;
    function setNextWindowFocus(): void;
    function setNextWindowBgAlpha(alpha: number): void;
    function setNextWindowViewport(viewportId: number): void;
    function setWindowPos(pos: Vec2, cond?: Cond): void;
    function setWindowSize(size: Vec2, cond?: Cond): void;
    function setWindowCollapsed(collapsed: boolean, cond?: Cond): void;
    function setWindowFocus(): void;
    function setWindowFontScale(scale: number): void;
    function setWindowPos(name: string, pos: Vec2, cond?: Cond): void;
    function setWindowSize(name: string, size: Vec2, cond?: Cond): void;
    function setWindowCollapsed(name: string, collapsed: boolean, cond?: Cond): void;
    function setWindowFocus(name: string): void;
    function getContentRegionAvail(): Vec2;
    function getContentRegionMax(): Vec2;
    function getWindowContentRegionMin(): Vec2;
    function getWindowContentRegionMax(): Vec2;
    function getScrollX(): number;
    function getScrollY(): number;
    function setScrollX(scrollX: number): void;
    function setScrollY(scrollY: number): void;
    function getScrollMaxX(): number;
    function getScrollMaxY(): number;
    function setScrollHereX(centerXRatio?: number): void;
    function setScrollHereY(centerYRatio?: number): void;
    function setScrollFromPosX(localX: number, centerXRatio?: number): void;
    function setScrollFromPosY(localY: number, centerYRatio?: number): void;
    function pushFont(font?: Font): void;
    function popFont(): void;
    function pushStyleColor(idx: Col, col: Vec4): void;
    function popStyleColor(count?: number): void;
    function pushStyleVar(idx: StyleVar, val: number): void;
    function pushStyleVar(idx: StyleVar, val: Vec2): void;
    function popStyleVar(count?: number): void;
    function pushAllowKeyboardFocus(allowKeyboardFocus: boolean): void;
    function popAllowKeyboardFocus(): void;
    function pushButtonRepeat(repeat: boolean): void;
    function popButtonRepeat(): void;
    function pushItemWidth(itemWidth: number): void;
    function popItemWidth(): void;
    function setNextItemWidth(itemWidth: number): void;
    function calcItemWidth(): number;
    function pushTextWrapPos(wrapLocalPosX?: number): void;
    function popTextWrapPos(): void;
    function getFont(): Font;
    function getFontSize(): number;
    function getFontTexUvWhitePixel(): Vec2;
    function getStyleColor(idx: Col): Vec4;
    function separator(): void;
    function sameLine(offsetFromStartX?: number, spacing?: number): void;
    function newLine(): void;
    function spacing(): void;
    function dummy(size: Vec2): void;
    function indent(indentW?: number): void;
    function unindent(indentW?: number): void;
    function beginGroup(): void;
    function endGroup(): void;
    function getCursorPos(): Vec2;
    function getCursorPosX(): number;
    function getCursorPosY(): number;
    function setCursorPos(localPos: Vec2): void;
    function setCursorPosX(localX: number): void;
    function setCursorPosY(localY: number): void;
    function getCursorStartPos(): Vec2;
    function getCursorScreenPos(): Vec2;
    function setCursorScreenPos(pos: Vec2): void;
    function alignTextToFramePadding(): void;
    function getTextLineHeight(): number;
    function getTextLineHeightWithSpacing(): number;
    function getFrameHeight(): number;
    function getFrameHeightWithSpacing(): number;
    function pushId(id: string): void;
    function pushId(id: OpaquePointer): void;
    function pushId(id: number): void;
    function popId(): void;
    function getId(id: string): number;
    function getId(id: OpaquePointer): number;
    function text(text: string): void;
    function textColored(col: Vec4, text: string): void;
    function textDisabled(text: string): void;
    function textWrapped(text: string): void;
    function labelText(label: string, text: string): void;
    function bulletText(text: string): void;
    function button(label: string, size?: Vec2): boolean;
    function smallButton(label: string): boolean;
    function invisibleButton(id: string, size: Vec2, flags?: ButtonFlags): boolean;
    function arrowButton(id: string, dir: Dir): boolean;
    function image(userTextureId: OpaquePointer, size: Vec2, uv0?: Vec2, uv1?: Vec2, tintCol?: Vec4, borderCol?: Vec4): void;
    function imageButton(userTextureId: OpaquePointer, size: Vec2, uv0?: Vec2, uv1?: Vec2, framePadding?: number, bgCol?: Vec4, tintCol?: Vec4): boolean;
    function checkbox(label: string, v: boolRef): boolean;
    function checkboxFlags(label: string, flags: numberRef, flagsValue: number): boolean;
    function radioButton(label: string, active: boolean): boolean;
    function radioButton(label: string, v: numberRef, vButton: number): boolean;
    function progressBar(fraction: number, sizeArg?: Vec2, overlay?: string): void;
    function bullet(): void;
    function beginCombo(label: string, previewValue: string, flags?: ComboFlags): boolean;
    function endCombo(): void;
    function combo(label: string, currentItem: numberRef, items: string[], popupMaxHeightInItems?: number): boolean;
    function combo(label: string, currentItem: numberRef, items: (index: number) => string, itemsCount: number, popupMaxHeightInItems?: number): boolean;
    function dragNumber(label: string, v: numberRef, vSpeed?: number, vMin?: number, vMax?: number, format?: string, flags?: SliderFlags): boolean;
    function dragNumbers(label: string, v: number[], vSpeed?: number, vMin?: number, vMax?: number, format?: string, flags?: SliderFlags): boolean;
    function dragNumberRange(label: string, vCurrentMin: numberRef, vCurrentMax: numberRef, vSpeed?: number, vMin?: number, vMax?: number, format?: string, formatMax?: string, flags?: SliderFlags): boolean;
    function sliderNumber(label: string, v: numberRef, vMin?: number, vMax?: number, format?: string, flags?: SliderFlags): boolean;
    function sliderNumbers(label: string, v: number[], vMin?: number, vMax?: number, format?: string, flags?: SliderFlags): boolean;
    function vSliderNumber(label: string, size: Vec2, v: numberRef, vMin?: number, vMax?: number, format?: string, flags?: SliderFlags): boolean;
    function sliderAngle(label: string, vRad: numberRef, vDegreesMin?: number, vDegreesMax?: number, format?: string, flags?: SliderFlags): boolean;
    function inputText(label: string, str: stringRef, flags?: InputTextFlags, callback?: (data: InputTextCallbackData) => number): boolean;
    function inputTextMultiline(label: string, str: stringRef, size?: Vec2, flags?: InputTextFlags, callback?: (data: InputTextCallbackData) => number): boolean;
    function inputTextWithHint(label: string, hint: string, str: stringRef, flags?: InputTextFlags, callback?: (data: InputTextCallbackData) => number): boolean;
    function inputNumber(label: string, v: numberRef, step?: number, stepFast?: number, format?: string, flags?: InputTextFlags): boolean;
    function inputNumbers(label: string, v: number[], step?: number, stepFast?: number, format?: string, flags?: InputTextFlags): boolean;
    function colorEdit3(label: string, col: Vec4, flags?: ColorEditFlags): boolean;
    function colorEdit4(label: string, col: Vec4, flags?: ColorEditFlags): boolean;
    function colorPicker3(label: string, col: Vec4, flags?: ColorEditFlags): boolean;
    function colorPicker4(label: string, col: Vec4, flags?: ColorEditFlags, refCol?: Vec4): boolean;
    function colorButton(descId: string, col: Vec4, flags?: ColorEditFlags, size?: Vec2): boolean;
    function setColorEditOptions(flags: ColorEditFlags): void;
    function treeNode(label: string): boolean;
    function treeNode(id: string, text: string): boolean;
    function treeNode(id: OpaquePointer, text: string): boolean;
    function treeNodeEx(label: string, flags?: TreeNodeFlags): boolean;
    function treeNodeEx(id: string, flags: TreeNodeFlags, text: string): boolean;
    function treeNodeEx(id: OpaquePointer, flags: TreeNodeFlags, text: string): boolean;
    function treePush(id: string): void;
    function treePush(id: OpaquePointer): void;
    function treePop(): void;
    function getTreeNodeToLabelSpacing(): number;
    function collapsingHeader(label: string, flags?: TreeNodeFlags): boolean;
    function collapsingHeader(label: string, visible: boolRef, flags?: TreeNodeFlags): boolean;
    function setNextItemOpen(isOpen: boolean, cond?: Cond): void;
    function selectable(label: string, selected?: boolean, flags?: SelectableFlags, size?: Vec2): boolean;
    function selectable(label: string, selected: boolRef, flags?: SelectableFlags, size?: Vec2): boolean;
    function beginListBox(label: string, size?: Vec2): boolean;
    function endListBox(): void;
    function listBox(label: string, currentItem: numberRef, items: string[], heightInItems?: number): boolean;
    function listBox(label: string, currentItem: numberRef, items: (index: number) => string, itemsCount: number, heightInItems?: number): boolean;
    function plotLines(label: string, values: number[], overlayText?: string, scaleMin?: number, scaleMax?: number, graphSize?: Vec2): void;
    function plotLines(label: string, values: (index: number) => number, valuesCount: number, overlayText?: string, scaleMin?: number, scaleMax?: number, graphSize?: Vec2): void;
    function plotHistogram(label: string, values: number[], overlayText?: string, scaleMin?: number, scaleMax?: number, graphSize?: Vec2): void;
    function plotHistogram(label: string, values: (index: number) => number, valuesCount: number, overlayText?: string, scaleMin?: number, scaleMax?: number, graphSize?: Vec2): void;
    function value(prefix: string, b: boolean): void;
    function value(prefix: string, v: number, format?: string): void;
    function beginMenuBar(): boolean;
    function endMenuBar(): void;
    function beginMainMenuBar(): boolean;
    function endMainMenuBar(): void;
    function beginMenu(label: string, enabled?: boolean): boolean;
    function endMenu(): void;
    function menuItem(label: string, shortcut?: string, selected?: boolean, enabled?: boolean): boolean;
    function menuItem(label: string, shortcut: string, selected: boolRef, enabled?: boolean): boolean;
    function beginTooltip(): void;
    function endTooltip(): void;
    function setTooltip(text: string): void;
    function beginPopup(id: string, flags?: WindowFlags): boolean;
    function beginPopupModal(name: string, open?: boolRef, flags?: WindowFlags): boolean;
    function endPopup(): void;
    function openPopup(id: string, flags?: PopupFlags): void;
    function openPopup(id: number, flags?: PopupFlags): void;
    function openPopupOnItemClick(id?: string, flags?: PopupFlags): void;
    function closeCurrentPopup(): void;
    function beginPopupContextItem(id?: string, flags?: PopupFlags): boolean;
    function beginPopupContextWindow(id?: string, flags?: PopupFlags): boolean;
    function beginPopupContextVoid(id?: string, flags?: PopupFlags): boolean;
    function isPopupOpen(id: string, flags?: PopupFlags): boolean;
    function beginTable(id: string, column: number, flags?: TableFlags, outerSize?: Vec2, innerWidth?: number): boolean;
    function endTable(): void;
    function tableNextRow(rowFlags?: TableRowFlags, minRowHeight?: number): void;
    function tableNextColumn(): boolean;
    function tableSetColumnIndex(columnN: number): boolean;
    function tableSetupColumn(label: string, flags?: TableColumnFlags, initWidthOrWeight?: number, userId?: number): void;
    function tableSetupScrollFreeze(cols: number, rows: number): void;
    function tableHeadersRow(): void;
    function tableHeader(label: string): void;
    function tableGetSortSpecs(): TableSortSpecs | undefined;
    function tableGetColumnCount(): number;
    function tableGetColumnIndex(): number;
    function tableGetRowIndex(): number;
    function tableGetColumnName(columnN?: number): string;
    function tableGetColumnFlags(columnN?: number): TableColumnFlags;
    function tableSetColumnEnabled(columnN: number, v: boolean): void;
    function tableSetBgColor(target: TableBgTarget, color: Vec4, columnN?: number): void;
    function columns(count?: number, id?: string, border?: boolean): void;
    function nextColumn(): void;
    function getColumnIndex(): number;
    function getColumnWidth(columnIndex?: number): number;
    function setColumnWidth(columnIndex: number, width: number): void;
    function getColumnOffset(columnIndex?: number): number;
    function setColumnOffset(columnIndex: number, offsetX: number): void;
    function getColumnsCount(): number;
    function beginTabBar(id: string, flags?: TabBarFlags): boolean;
    function endTabBar(): void;
    function beginTabItem(label: string, open?: boolRef, flags?: TabItemFlags): boolean;
    function endTabItem(): void;
    function tabItemButton(label: string, flags?: TabItemFlags): boolean;
    function setTabItemClosed(tabOrDockedWindowLabel: string): void;
    function dockSpace(id: number, size?: Vec2, flags?: number, windowClass?: WindowClass): number;
    function dockSpaceOverViewport(viewport?: Viewport, flags?: number, windowClass?: WindowClass): number;
    function setNextWindowDockId(dockId: number, cond?: Cond): void;
    function setNextWindowClass(windowClass: WindowClass): void;
    function getWindowDockId(): number;
    function isWindowDocked(): boolean;
    function logToTTY(autoOpenDepth?: number): void;
    function logToFile(autoOpenDepth?: number, filename?: string): void;
    function logToClipboard(autoOpenDepth?: number): void;
    function logFinish(): void;
    function logButtons(): void;
    function logText(text: string): void;
    function beginDragDropSource(flags?: DragDropFlags): boolean;
    function setDragDropPayload(type: string, data: ArrayBuffer, cond?: Cond): boolean;
    function endDragDropSource(): void;
    function beginDragDropTarget(): boolean;
    function acceptDragDropPayload(type: string, flags?: DragDropFlags): Payload;
    function endDragDropTarget(): void;
    function getDragDropPayload(): Payload;
    function beginDisabled(disabled?: boolean): void;
    function endDisabled(): void;
    function pushClipRect(clipRectMin: Vec2, clipRectMax: Vec2, intersectWithCurrentClipRect: boolean): void;
    function popClipRect(): void;
    function setItemDefaultFocus(): void;
    function setKeyboardFocusHere(offset?: number): void;
    function isItemHovered(flags?: HoveredFlags): boolean;
    function isItemActive(): boolean;
    function isItemFocused(): boolean;
    function isItemClicked(mouseButton?: MouseButton): boolean;
    function isItemVisible(): boolean;
    function isItemEdited(): boolean;
    function isItemActivated(): boolean;
    function isItemDeactivated(): boolean;
    function isItemDeactivatedAfterEdit(): boolean;
    function isItemToggledOpen(): boolean;
    function isAnyItemHovered(): boolean;
    function isAnyItemActive(): boolean;
    function isAnyItemFocused(): boolean;
    function getItemRectMin(): Vec2;
    function getItemRectMax(): Vec2;
    function getItemRectSize(): Vec2;
    function setItemAllowOverlap(): void;
    function getMainViewport(): Viewport;
    function getBackgroundDrawList(): DrawList;
    function getForegroundDrawList(): DrawList;
    function getBackgroundDrawList(viewport: Viewport): DrawList;
    function getForegroundDrawList(viewport: Viewport): DrawList;
    function isRectVisible(size: Vec2): boolean;
    function isRectVisible(rectMin: Vec2, rectMax: Vec2): boolean;
    function getTime(): number;
    function getFrameCount(): number;
    function getStyleColorName(idx: Col): string;
    function beginChildFrame(id: number, size: Vec2, flags?: WindowFlags): boolean;
    function endChildFrame(): void;
    function calcTextSize(text: string, hideTextAfterDoubleHash?: boolean, wrapWidth?: number): Vec2;
    function colorConvertRGBtoHSV(rgb: Vec3): Vec3;
    function colorConvertHSVtoRGB(hsv: Vec3): Vec3;
    function isKeyDown(key: Key): boolean;
    function isKeyPressed(key: Key, repeat?: boolean): boolean;
    function isKeyReleased(key: Key): boolean;
    function getKeyPressedAmount(key: Key, repeatDelay: number, rate: number): number;
    function getKeyName(key: Key): string;
    function setNextFrameWantCaptureKeyboard(wantCaptureKeyboard: boolean): void;
    function isMouseDown(button: MouseButton): boolean
    function isMouseClicked(button: MouseButton, repeat?: boolean): boolean;
    function isMouseReleased(button: MouseButton): boolean;
    function isMouseDoubleClicked(button: MouseButton): boolean;
    function getMouseClickedCount(button: MouseButton): number;
    function isMouseHoveringRect(rMin: Vec2, rMax: Vec2, clip?: boolean): boolean;
    function isMousePosValid(mousePos?: Vec2): boolean;
    function isAnyMouseDown(): boolean
    function getMousePos(): Vec2;
    function getMousePosOnOpeningCurrentPopup(): Vec2;
    function isMouseDragging(button: MouseButton, lockThreshold?: number): boolean;
    function getMouseDragDelta(button?: MouseButton, lockThreshold?: number): Vec2;
    function resetMouseDragDelta(button?: MouseButton): void;
    function getMouseCursor(): MouseCursor;
    function setMouseCursor(cursorType: MouseCursor): void;
    function setNextFrameWantCaptureMouse(wantCaptureMouse: boolean): void;
    function getClipboardText(): string;
    function setClipboardText(text: string): void;
    function loadIniSettingsFromFile(iniFilename: string): void;
    function loadIniSettingsFromString(iniData: string): void;
    function saveIniSettingsToFile(iniFilename: string): void;
    function saveIniSettingsToString(): string;
    function findViewportById(id: number): Viewport;
    function findViewportByPlatformHandle(platformHandle: OpaquePointer): Viewport;
    function internalSetItemUsingMouseWheel(): void;
}

export module Gui {
    type FileDialogFilters = (string | string[])[];

    interface FileDialogOptions {
        parent?: ImGui.Viewport;
        title?: string;
        currentFolder?: string;
        filters?: FileDialogFilters;
    }

    interface FolderDialogOptions {
        parent?: ImGui.Viewport;
        title?: string;
        currentFolder?: string;
    }

    interface FileDialogResult {
        selectedFilter?: number;
        filename?: string;
    }

    interface FileDialogResults {
        selectedFilter?: number;
        filenames?: string[];
    }

    type FolderDialogResult = string | undefined;
    type FolderDialogResults = string[] | undefined;

    function getOpenFileName(options?: FileDialogOptions): FileDialogResult;
    function getOpenFileNames(options?: FileDialogOptions): FileDialogResults;
    function getSaveFileName(options?: FileDialogOptions): FileDialogResult;
    function getExistingDirectory(options?: FolderDialogOptions): FolderDialogResult;
    function getExistingDirectories(options?: FolderDialogOptions): FolderDialogResults;

    const enum MessageBoxType {
        Other,
        Warning,
        Error,
        Information
    }

    const enum MessageBoxButtons {
        OK,
        Cancel,
        OKCancel,
        YesNo,
        Close
    }

    const enum MessageBoxResponse {
        OK,
        Cancel,
        Yes,
        No,
        Close
    }

    interface MessageBoxOptions {
        parent?: ImGui.Viewport;
        title?: string;
        subtitle?: string;
        content?: string;
        type?: MessageBoxType;
        buttons?: MessageBoxButtons;
    }

    function showMessageBox(options?: MessageBoxOptions): MessageBoxResponse;

    const enum ColorScheme {
        PreferLight,
        PreferDark
    }

    function getColorScheme(): ColorScheme;
    function getAccentColor(): Vec4;
}

export module Gfx {
    interface Texture {
        equals(other: Texture): boolean;
        loadFromFile(filename: string): void;
        loadFromMemory(data: Uint8Array): void;
        loadFromRGBA32(width: number, height: number, data: Uint8Array): void;
        id: OpaquePointer;
        size: Vec2;
    }

    export var Texture: {
        new(): Texture;
    };

    interface Font {
        equals(other: Font): boolean;
        loadFromFile(filename: string, ptSize: number): void;
        loadFromMemory(data: Uint8Array, ptSize: number): void;
        fillColor: Vec4;
        outlineColor: Vec4;
        outlineThickness: number;
        renderText(text: string): Texture;
    }

    export var Font: {
        new(): Font;
    };
}

type AppCallbackName = 'init' | 'before-new-frame' | 'new-frame' | 'before-render' | 'render' | 'drop-file';
type AppCallbackVoid = () => void;
type AppCallbackDropFile = (filename: string, viewport: ImGui.Viewport) => void;

interface AppWindow {
    equals(other: AppWindow): boolean;
    on(name: AppCallbackName, callback?: AppCallbackVoid | AppCallbackDropFile): void;
    doIteration(): boolean;
    title: string;
    setIconFromFile(filename: string): void;
    setIconFromMemory(data: ArrayBuffer): void;
    size: Vec2;
    minimumSize: Vec2;
    maximumSize: Vec2;
    clearColor: Vec4;
}

export var AppWindow: {
    new(): AppWindow;
};
